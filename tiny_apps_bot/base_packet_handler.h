#ifndef BASE_PACKET_HANDLER_H
#define BASE_PACKET_HANDLER_H

#include "i_packet_handler.h"
#include "i_handlers_factory.h"


namespace handlers {
namespace http {


/** @brief Used as a base class for all packet handlers. Keeps pointer to the factory and implement the next handler method. (CoR pattern) */
class BasePacketHandler : public IPacketHandler {
public:
	BasePacketHandler() = default;
	explicit BasePacketHandler(IHandlersFactory* factory_ptr) 
			: factory_ptr_{ factory_ptr } {
	}
	virtual ~BasePacketHandler() = default;

	virtual ResponseStrBody HandlePacket(const RequestStrBody&) = 0;
	virtual void SetNextHandler(IPacketHandlerPtr packet_handler_ptr) final {
		next_packet_handler_ptr_ = packet_handler_ptr;
	}
protected:
	IPacketHandlerPtr next_packet_handler_ptr_ = nullptr;
	IHandlersFactory* factory_ptr_ = nullptr;
};


}  // namespace http
}  // namespace handlers

#endif  // BASE_PACKET_HANDLER_H