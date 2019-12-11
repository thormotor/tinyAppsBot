#ifndef CLIENT_HTTP_SESSION_H
#define CLIENT_HTTP_SESSION_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

#include "logger.h"


namespace network {
namespace client {

using boost::asio::io_context;
using boost::system::error_code;
using RequestStrBody = boost::beast::http::request<boost::beast::http::string_body>;
using ResponseStrBody = boost::beast::http::response<boost::beast::http::string_body>;
using SslSocket = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;


/** @brief Maintains data exchange communication process. Performs outgoing connections to the messenger's servers. */
class ClientHttpSession : public std::enable_shared_from_this<ClientHttpSession> {
 public:
	explicit ClientHttpSession(io_context&, boost::asio::ssl::context&);
	~ClientHttpSession() = default;
	void Run(RequestStrBody request, boost::asio::ip::tcp::resolver::results_type results);
	void HandleConnect(error_code ec);
	void HandleHandshake(error_code ec);
	void HandleWrite(error_code ec, std::size_t bytes_transferred);
	void HandleRead(error_code ec, std::size_t bytes_transferred);
	void HandleShutdown(error_code ec);
 private:
	SslSocket ssl_socket_;
	boost::beast::flat_buffer buffer_; // (Must persist between reads)
	ResponseStrBody res_;
	RequestStrBody req_;
	std::shared_ptr<spdlog::logger> log = spdlog::get(LOG_CLIENT);
};


}  // namespace client
}  // namespace network

#endif  // CLIENT_HTTP_SESSION_H