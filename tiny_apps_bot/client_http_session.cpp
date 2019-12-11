#include "client_http_session.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "vld.h"


namespace network {
namespace client {

namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;
using boost::asio::ip::tcp;
namespace ph = std::placeholders;



ClientHttpSession::ClientHttpSession(io_context& io_service, ssl::context& ssl_context) 
		: ssl_socket_{io_service, ssl_context} 
{
	log->trace(u8"ClientSession: created new <CLientSession> object");
}


void ClientHttpSession::Run(RequestStrBody request, tcp::resolver::results_type results) {
	this->req_ = request;
	// Set SNI Hostname (many hosts need this to handshake successfully)
	if (!SSL_set_tlsext_host_name(ssl_socket_.native_handle(), std::string(req_.at(http::field::host)).c_str())) {
		error_code error{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
		log->debug("SSL_set_tlsext_host_name: {}", error.message());
		return;
	}
	// Making a connection to IP address we have got by lookup
	boost::asio::async_connect(ssl_socket_.next_layer(), results.begin(), results.end(),
		std::bind(&ClientHttpSession::HandleConnect, shared_from_this(), ph::_1));
	log->trace("ClientSession: start async connect");
}


void ClientHttpSession::HandleConnect(error_code error) {
	if (error) return log->debug("ClientSession connection failure: ", error.message());
	// Perform the SSL handshake
	ssl_socket_.async_handshake(ssl::stream_base::client,
		std::bind(&ClientHttpSession::HandleHandshake, shared_from_this(), ph::_1));
	log->trace("ClientSession: start async handshake");
}


void ClientHttpSession::HandleHandshake(error_code error) {
	if (error) return log->debug("ClientSession: handshake failure ({}). Error code: ", error.message(), ERR_get_error());
	// Send the HTTP request to the remote host
	spdlog::get(LOG_DATA)->info("clt> sent body: {}", req_.body());
	http::async_write(ssl_socket_, req_,
			std::bind(&ClientHttpSession::HandleWrite, shared_from_this(), ph::_1, ph::_2));
	log->trace("ClientSession: start async write");
	//log->trace(req_.body());
}


void ClientHttpSession::HandleWrite(error_code error, std::size_t bytes_transferred) {
	boost::ignore_unused(bytes_transferred);
	if (error) return log->debug("ClientSession write failure: ", error.message());
	// Receive the HTTP response
	http::async_read(ssl_socket_, buffer_, res_,
			std::bind(&ClientHttpSession::HandleRead, shared_from_this(), ph::_1, ph::_2));
	log->trace("ClientSession: start async read");
}


void ClientHttpSession::HandleRead(error_code error, std::size_t bytes_transferred) {
	boost::ignore_unused(bytes_transferred);
	if (error) return log->debug("ClientSession read failure: ", error.message());
	spdlog::get(LOG_DATA)->info("clt< received body: {}", res_.body());
	// Gracefully close the stream
	ssl_socket_.async_shutdown(std::bind(&ClientHttpSession::HandleShutdown, shared_from_this(), ph::_1));
	log->trace("ClientSession: start async shutdown");
}


void ClientHttpSession::HandleShutdown(error_code error) {
	if (error == boost::asio::error::eof) {
		// Rationale:
		// http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
		error.assign(0, error.category());
	} else if (error) {
		return log->debug("ClientSession shutdown failure: ", error.message());
	}
	// If we get here then the connection has closed gracefully
}


}  // namespace client
}  // namespace network