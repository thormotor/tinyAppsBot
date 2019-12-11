#ifndef SERVER_HTTP_H
#define SERVER_HTTP_H

#include <memory>
#include <set>
#include <stdint.h>
#include <boost/beast.hpp>

#include "i_server_http.h"
#include "logger.h"
#include "i_network_helper.h"


namespace network {
namespace server {

using boost::asio::ip::tcp;
using boost::system::error_code;
using SslSocket = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;
//using RequestStrBody = boost::beast::http::request<boost::beast::http::string_body>;
//using ResponseStrBody = boost::beast::http::response<boost::beast::http::string_body>;
//using RequestHandler = std::function<ResponseStrBody(const RequestStrBody&)>;


/** @brief Waits for incoming connections and when it occurs creates ServerHttpSession object for handling. */
class ServerHttp : public IServerHttp {
public:
	explicit ServerHttp(INetworkHelperPtr network_helper, uint16_t port);
	~ServerHttp() = default;

	void Subscribe(handlers::http::IPacketHandlerPtr packet_handler) override;
	void Unsubscribe() override;
private:
	void StartAccept();
	void HandleAccept(std::shared_ptr<SslSocket> ssl_socket, const error_code& error);

	const uint16_t port_;
	INetworkHelperPtr network_helper_;
	tcp::acceptor acceptor_;
	//RequestHandler extern_req_handler_;
	handlers::http::IPacketHandlerPtr extern_req_handler_;
	std::shared_ptr<spdlog::logger> log = spdlog::get(LOG_SERVER);
};


}  // namespace server
}  // namespace network

#endif  // SERVER_HTTP_H