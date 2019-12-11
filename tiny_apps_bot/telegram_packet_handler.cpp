#include "telegram_packet_handler.h"

#include "main_config.h"
#include "logger.h"


namespace handlers {
namespace http {

using boost::property_tree::ptree;


TelegramPacketHandler::TelegramPacketHandler(IHandlersFactory* factory_ptr)
	: BasePacketHandler(factory_ptr),
	webhook_comparison_string_(MainConfig::getInstance()->getParam("messengers.telegram.webhook_target_comparison_string", std::string{ "/tlgrm" }))
{
}


ResponseStrBody TelegramPacketHandler::HandlePacket(const RequestStrBody& request_packet) {
	ResponseStrBody response_packet;  // response by default (http 200 OK)
	if (request_packet.target().to_string().find(webhook_comparison_string_) != std::string::npos) {
		// it's a telegram packet - handle it
		auto post_data = DeserializeJson(request_packet.body());
		auto event_name = post_data.get<std::string>("event", "");
		auto event_handler = factory_ptr_->GetEventHandler(event_name);
		event_handler->Execute(post_data);
	} else {
		// pass the packet to the next handler
		response_packet = next_packet_handler_ptr_->HandlePacket(request_packet);
	}
	return response_packet;
}


ptree TelegramPacketHandler::DeserializeJson(const std::string& json_string) {
	ptree event_data;
	try {
		boost::property_tree::read_json(std::stringstream{json_string}, event_data);
	} catch (const std::exception& err) {
		spdlog::get(LOG_MAIN)->debug("json message convertion failure : {}", err.what());
	}
	return event_data;
}


}  // namespace http
}  // namespace handlers