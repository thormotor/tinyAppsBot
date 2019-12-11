#ifndef VIBER_PACKET_HANDLER_H
#define VIBER_PACKET_HANDLER_H

#include "base_packet_handler.h"
#include "viber_event_handler.h"


namespace handlers {
namespace http {

using boost::property_tree::ptree;


/** @brief Determines if the incoming HTTP packet is a Viber packet. In that case, deserializes its body and call Event Handler. (CoR pattern) */
class ViberPacketHandler : public BasePacketHandler {
public:
	ViberPacketHandler(IHandlersFactory* factory_ptr);
	~ViberPacketHandler() = default;

	ResponseStrBody HandlePacket(const RequestStrBody& request_packet) override;
private:
	ptree DeserializeJson(const std::string& json_string);

	const std::string webhook_comparison_string_;
};
	
	
}  // namespace http
}  // namespace handlers

#endif  // VIBER_PACKET_HANDLER_H