#ifndef FINAL_PACKET_HANDLER_H
#define FINAL_PACKET_HANDLER_H

#include "base_packet_handler.h"


namespace handlers {
namespace http {


/** @brief The last handler in the chain. It's used in case no one processed the incoming packet. (CoR pattern) */
class FinalPacketHandler :	public BasePacketHandler {
public:
	FinalPacketHandler() = default;
	FinalPacketHandler(const FinalPacketHandler&) = delete;
	FinalPacketHandler& operator=(const FinalPacketHandler&) = delete;
	~FinalPacketHandler() = default;
	ResponseStrBody HandlePacket(const RequestStrBody& request_packet) override;
};


}  // namespace http
}  // namespace handlers

#endif  // FINAL_PACKET_HANDLER_H