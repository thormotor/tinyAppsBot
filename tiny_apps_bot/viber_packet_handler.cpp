#include "viber_packet_handler.h"

#include "main_config.h"
#include "logger.h"


namespace handlers {
namespace http {

using boost::property_tree::ptree;


ViberPacketHandler::ViberPacketHandler(IHandlersFactory* factory_ptr) 
	: BasePacketHandler(factory_ptr), 
	webhook_comparison_string_(MainConfig::getInstance()->getParam("messengers.viber.webhook_target_comparison_string", std::string{"/viberbot?"}))
{
}


ResponseStrBody ViberPacketHandler::HandlePacket(const RequestStrBody& request_packet) {
	ResponseStrBody response_packet;  // response by default (http 200 OK)
	if (request_packet.target().to_string().find(webhook_comparison_string_) != std::string::npos) {
		// it's a viber packet - handle it
		auto post_data = DeserializeJson(request_packet.body());
		auto event_name = post_data.get<std::string>("event", "");
		auto event_handler = factory_ptr_->GetEventHandler(event_name);
		response_packet.body() = event_handler->Execute(post_data);
		response_packet.prepare_payload();
	} else {
		// pass the packet to the next handler
		response_packet = next_packet_handler_ptr_->HandlePacket(request_packet);
	}
	return response_packet;
}


ptree ViberPacketHandler::DeserializeJson(const std::string& json_string) {
	ptree event_data;
	try {
		boost::property_tree::read_json(std::stringstream{json_string}, event_data);
	} catch (const std::exception& err) {
		spdlog::get(LOG_MAIN)->debug("json message convertion failure : {}", err.what());
	}
	return event_data;
}


}  // amespace http
}  // namespace handlers