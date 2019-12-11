#ifndef NETWORK_HELPER_H
#define NETWORK_HELPER_H

#include "i_network_helper.h"

#include <memory>
#include <future>
#include <vector>

#include "logger.h"

namespace network {


/** @brief Configures common network environment "boost::asio" for either server and client. */
class NetworkHelper : public INetworkHelper {
public:
	explicit NetworkHelper(const std::string& certificates_folder_path, int number_of_threads);
	~NetworkHelper() = default;

	ssl::context& GetSslContext() override;
	io_context& GetIoService() override;
	tcp::resolver::results_type ResolveHost(const std::string& host_name) override;
	void Stop() override;
private:
	void LoadCertificates(const std::string& certificates_folder_path);
	void StartIoServiceThreads();
	void StopIoServiceThreads();

	int number_of_threads_;
	ssl::context ssl_context_;
	io_context io_service_;
	boost::asio::executor_work_guard<io_context::executor_type> io_service_work_guard_
		= boost::asio::make_work_guard(io_service_);  // Guards the io_service_ against finishing execution when it out of work
	std::vector<std::future<void>> io_service_thread_pool;
	std::shared_ptr<spdlog::logger> log = spdlog::get(LOG_SERVER);
};


}  // namespace network

#endif  // NETWORK_HELPER_H