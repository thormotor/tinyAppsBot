#ifndef CLIENT_HTTP_H
#define CLIENT_HTTP_H

#include "i_client_http.h"
#include "logger.h"
#include "i_network_helper.h"


namespace network {
namespace client {


/** @brief Prepares outgoing connections. Creates ClientHttpSession object and passes them HTTP requests for sending. */
class ClientHttp : public IClientHttp {
public:
	ClientHttp(const INetworkHelperPtr& network_helper);
	~ClientHttp() = default;
	void SendPacket(RequestStrBody request_packet) override;
private:
	INetworkHelperPtr network_helper_;
	std::shared_ptr<spdlog::logger> log = spdlog::get(LOG_CLIENT);
};
	
	
}  // amespace client
}  // namespace network

#endif  // CLIENT_HTTP_H