#include "server_http_session.h"

#include "vld.h"


namespace network {
namespace server {

namespace ph = std::placeholders;
namespace http = boost::beast::http;

size_t ServerHttpSession::instances_counter_ = 0;	// starting id_ number


ServerHttpSession::ServerHttpSession(std::shared_ptr<SslSocket> ssl_socket, handlers::http::IPacketHandlerPtr extern_req_handler)
	: ssl_socket_{ ssl_socket }, extern_req_handler_{ extern_req_handler }, id_(++instances_counter_) 
{
	log->trace("object ServerHttpSession #{} created", id_);
}


ServerHttpSession::~ServerHttpSession() {
	log->trace("object ServerHttpSession #{} deleted", id_);
}


void ServerHttpSession::StartCommunication() {
	ssl_socket_->async_handshake(boost::asio::ssl::stream_base::server,
			std::bind(&ServerHttpSession::HandleHandshake, shared_from_this(), ph::_1));
}


void ServerHttpSession::WriteRequestToDataLog(const RequestStrBody& request_packet) const {
	spdlog::get(LOG_DATA)->info(u8"srv< received target: \"{}\"", request_packet.target().to_string());
	std::string header;
	for (auto it = request_packet.cbegin(); it != request_packet.cend(); ++it)
		header += it->name_string().to_string() + ": " + it->value().to_string() + "; ";
	spdlog::get(LOG_DATA)->info(u8"srv< received header | {}", header);
	spdlog::get(LOG_DATA)->info(u8"srv< received body | {}", request_packet.body());
}


void ServerHttpSession::WriteResponseToDataLog(const ResponseStrBody& response_packet) const {
	spdlog::get(LOG_DATA)->info("srv> sent body: {}", response_packet.body());
}


void ServerHttpSession::HandleHandshake(const error_code& error) {
	if (error) return log->debug("ServerHttpSession #{}: handshake failure: ({})", id_, error.message());
	http::async_read(*ssl_socket_, buffer_, req_,
			std::bind(&ServerHttpSession::HandleRead, shared_from_this(), ph::_1, ph::_2));
}


void ServerHttpSession::HandleRead(const error_code & error, size_t bytes_transferred) {
	if (error) return log->debug("ServerHttpSession #{}: read failure: ({})", id_, error.message());

	WriteRequestToDataLog(req_);
	//---------------------------------------------------------------
	log->trace("ServerHttpSession #{}: invoking external request handler", id_);
	if (extern_req_handler_) {
		VLDEnable();
		res_ = extern_req_handler_->HandlePacket(req_);
		VLDDisable();
	}
	//---------------------------------------------------------------
	WriteResponseToDataLog(res_);
	http::async_write(*ssl_socket_, res_,
			std::bind(&ServerHttpSession::HandleWrite, shared_from_this(), ph::_1));
}


void ServerHttpSession::HandleWrite(const error_code & error) {
	if (error) return log->debug("ServerHttpSession #{}: write failure: ({})", id_, error.message());
	log->trace("ServerHttpSession #{}: write is ok", id_);
}


}  //namespace server
}  //namespace network
