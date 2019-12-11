#ifndef TELEGRAM_PACKET_HANDLER_H
#define TELEGRAM_PACKET_HANDLER_H

#include "base_packet_handler.h"
#include "telegram_event_handler.h"


namespace handlers {
namespace http {

using boost::property_tree::ptree;


/** @brief Determines if the incoming HTTP packet is a Telegram packet. In that case, deserializes its body and call Event Handler. (CoR pattern) */
class TelegramPacketHandler : public BasePacketHandler {
public:
	TelegramPacketHandler(IHandlersFactory* factory_ptr);
	~TelegramPacketHandler() = default;

	ResponseStrBody HandlePacket(const RequestStrBody& request_packet) override;
private:
	ptree DeserializeJson(const std::string& json_string);

	const std::string webhook_comparison_string_;
};


}  // namespace http
}  // namespace handlers

#endif  // TELEGRAM_PACKET_HANDLER_H