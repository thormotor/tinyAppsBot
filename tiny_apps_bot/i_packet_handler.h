#ifndef I_PACKET_HANDLER_H
#define I_PACKET_HANDLER_H

#include <boost/beast.hpp>

namespace handlers {
/** @brief Contains handlers for HTTP packets. */
namespace http {
	
	
using RequestStrBody = boost::beast::http::request<boost::beast::http::string_body>;
using ResponseStrBody = boost::beast::http::response<boost::beast::http::string_body>;


/** @brief Declares interface of each handler in the chain of packet handlers. (CoR pattern) */
class IPacketHandler {
public:
	IPacketHandler(const IPacketHandler&) = delete;
	IPacketHandler& operator=(const IPacketHandler&) = delete;
	IPacketHandler(IPacketHandler&&) = delete;
	IPacketHandler& operator=(IPacketHandler&&) = delete;

	virtual ResponseStrBody HandlePacket(const RequestStrBody& request_packet) = 0;
	virtual void SetNextHandler(std::shared_ptr<IPacketHandler> packet_handler) = 0;
protected:
	IPacketHandler() = default;
	virtual ~IPacketHandler() = default;
};

using IPacketHandlerPtr = std::shared_ptr<IPacketHandler>;


}  // namespace http
}  // namespace handlers

#endif  // I_PACKET_HANDLER_H