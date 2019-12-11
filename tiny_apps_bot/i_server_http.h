#ifndef I_SERVER_HTTP_H
#define I_SERVER_HTTP_H

#include "i_packet_handler.h"

namespace network {
/** @brief Contains classes to support incoming connections */
namespace server {


/** @brief Declares interface of HTTP server object to handle incoming connections  */
class IServerHttp {
public:
	IServerHttp(const IServerHttp&) = delete;
	IServerHttp& operator=(const IServerHttp&) = delete;
	IServerHttp(IServerHttp&&) = delete;
	IServerHttp& operator=(IServerHttp&&) = delete;

	virtual void Subscribe(handlers::http::IPacketHandlerPtr packet_handler) = 0;
	virtual void Unsubscribe() = 0;
protected:
	IServerHttp() = default;
	virtual ~IServerHttp() = default;
};

using IServerHttpPtr = std::shared_ptr<IServerHttp>;


}  // namespace server
}  // namespace network

#endif  // I_SERVER_HTTP_H