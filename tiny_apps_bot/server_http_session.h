#ifndef SERVER_HTTP_SESSION_H
#define SERVER_HTTP_SESSION_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

#include "logger.h"
#include "i_packet_handler.h"


namespace network {
namespace server {

using boost::system::error_code;
using RequestStrBody = boost::beast::http::request<boost::beast::http::string_body>;
using ResponseStrBody = boost::beast::http::response<boost::beast::http::string_body>;
using SslSocket = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;


/** @brief Maintains data exchange communication process. Receives HTTP requests and pass them to the handler. */
class ServerHttpSession : public std::enable_shared_from_this<ServerHttpSession> {
public:
	explicit ServerHttpSession(std::shared_ptr<SslSocket> ssl_socket, handlers::http::IPacketHandlerPtr extern_req_handler);
	ServerHttpSession(const ServerHttpSession&) = delete;
	ServerHttpSession& operator=(const ServerHttpSession&) = delete;
	~ServerHttpSession();

	void StartCommunication();
private:
	void WriteRequestToDataLog(const RequestStrBody& request_packet) const;
	void WriteResponseToDataLog(const ResponseStrBody& response_packet) const;
	void HandleHandshake(const error_code& error);
	void HandleRead(const error_code& error, size_t bytes_transferred);
	void HandleWrite(const error_code& error);

	static size_t instances_counter_;
	const size_t id_;
	std::shared_ptr<SslSocket> ssl_socket_;
	RequestStrBody req_;
	ResponseStrBody res_;
	boost::beast::flat_buffer buffer_;
	handlers::http::IPacketHandlerPtr extern_req_handler_;
	std::shared_ptr<spdlog::logger> log = spdlog::get(LOG_SERVER);
};


}  // namespace server
}  // namespace network

#endif  // SERVER_HTTP_SESSION_H