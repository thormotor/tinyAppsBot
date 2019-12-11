#ifndef I_CLIENT_HTTP_H
#define I_CLIENT_HTTP_H

#include <boost/beast.hpp>	

namespace network {
/** @brief Contains classes to support outgoing connections */
namespace client {

using RequestStrBody = boost::beast::http::request<boost::beast::http::string_body>;
using ResponseStrBody = boost::beast::http::response<boost::beast::http::string_body>;


/** @brief Declares interface of HTTP client object to perform outgoing connections. */
class IClientHttp {
public:
	IClientHttp(const IClientHttp&) = delete;
	IClientHttp& operator=(const IClientHttp&) = delete;
	IClientHttp(IClientHttp&&) = delete;
	IClientHttp& operator=(IClientHttp&&) = delete;

	virtual void SendPacket(RequestStrBody request_packet) = 0;
protected:
	IClientHttp() = default;
	virtual ~IClientHttp() = default;
};

using IClientHttpPtr = std::shared_ptr<IClientHttp>;


}  // amespace client
}  // namespace network

#endif  // I_CLIENT_HTTP_H