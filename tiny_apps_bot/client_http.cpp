#include "client_http.h"

#include "client_http_session.h"


namespace network {
namespace client {

ClientHttp::ClientHttp(const INetworkHelperPtr& network_helper)
		: network_helper_{network_helper} 
{
}


void ClientHttp::SendPacket(RequestStrBody request_packet) {
	auto resolve_iterator = network_helper_->ResolveHost(request_packet.at(boost::beast::http::field::host).to_string());
	auto client_session = std::make_shared<ClientHttpSession>(network_helper_->GetIoService(), network_helper_->GetSslContext());
	client_session->Run(request_packet, resolve_iterator);
}
	
}  // namespace client
}  // namespace network