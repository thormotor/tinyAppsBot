#ifndef I_NETWORK_HELPER_H
#define I_NETWORK_HELPER_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

/** @brief Contains network handling classes */
namespace network {

namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;
using boost::asio::io_context;


/** @brief Declares interface of network helper that manages network environment. */
class INetworkHelper {
public:
	INetworkHelper(const INetworkHelper&) = delete;
	INetworkHelper& operator=(const INetworkHelper&) = delete;
	INetworkHelper(INetworkHelper&&) = delete;
	INetworkHelper& operator=(INetworkHelper&&) = delete;

	virtual ssl::context& GetSslContext() = 0;
	virtual io_context& GetIoService() = 0;
	virtual tcp::resolver::results_type ResolveHost(const std::string& host_name) = 0;
	virtual void Stop() = 0;
protected:
	INetworkHelper() = default;
	virtual ~INetworkHelper() = default;
};

using INetworkHelperPtr = std::shared_ptr<INetworkHelper>;


}  // namespace network

#endif  // I_NETWORK_HELPER_H