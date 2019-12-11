#include "server_http.h"
#include "server_http_session.h"

#include "vld.h"


namespace network {
namespace server {

namespace ph = std::placeholders;


ServerHttp::ServerHttp(INetworkHelperPtr network_helper, uint16_t port)
		: network_helper_{ network_helper }, 
		acceptor_{network_helper->GetIoService(), tcp::endpoint(tcp::v4(), port)},
		port_{port}
{
	StartAccept();
}

void ServerHttp::Subscribe(handlers::http::IPacketHandlerPtr packet_handler) {
	extern_req_handler_ = packet_handler;
}

void ServerHttp::Unsubscribe() {
	extern_req_handler_.reset();
}


void ServerHttp::StartAccept() {
	auto ssl_socket = std::make_shared<SslSocket>(network_helper_->GetIoService(), network_helper_->GetSslContext());
	acceptor_.async_accept(ssl_socket->lowest_layer(), std::bind(&ServerHttp::HandleAccept, this, ssl_socket, ph::_1));
}


void ServerHttp::HandleAccept(std::shared_ptr<SslSocket> ssl_socket, const error_code & error) {
	if (!error)	{
		std::string remote_address = ssl_socket->lowest_layer().remote_endpoint().address().to_string();
		log->debug("incoming connection from {}", remote_address);
		std::make_shared<ServerHttpSession>(ssl_socket, extern_req_handler_)->StartCommunication();
	} else {
		log->debug("async accept failure: {}", error.message());
	}
	StartAccept();
}


}  // namespace server
}  // namespace network