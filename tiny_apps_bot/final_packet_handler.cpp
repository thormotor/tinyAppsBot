#include "final_packet_handler.h"

#include "logger.h"


namespace handlers {
namespace http {
	
namespace http = boost::beast::http;


ResponseStrBody FinalPacketHandler::HandlePacket(const RequestStrBody& request_packet) {
	spdlog::get(LOG_MAIN)->trace(u8"FinalPacketHandler. Appropriate handler not found. 400 Bad Request.");
	ResponseStrBody response_packet;
	response_packet.result(http::status::bad_request);
	response_packet.version(request_packet.version());
	response_packet.set(http::field::server, BOOST_BEAST_VERSION_STRING);
	response_packet.set(http::field::content_type, "text/html");
	response_packet.keep_alive(request_packet.keep_alive());
	response_packet.body() = "Bad Request";
	response_packet.prepare_payload();
	return response_packet;
}


}  // namespace http
}  // namespace handlers