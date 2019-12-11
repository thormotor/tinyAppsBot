#include "network_helper.h"

#include "boost/asio/executor_work_guard.hpp"

#include "vld.h"

namespace network {

NetworkHelper::NetworkHelper(const std::string& certificates_folder_path, int number_of_threads)
		: ssl_context_(ssl::context::sslv23), number_of_threads_(number_of_threads)
{
	ssl_context_.set_options(	ssl::context::default_workarounds |
								ssl::context::no_sslv2 |
								ssl::context::single_dh_use);
	LoadCertificates(certificates_folder_path);
	StartIoServiceThreads();
}


void NetworkHelper::LoadCertificates(const std::string& certificates_folder_path) {
	try {
		ssl_context_.use_certificate_chain_file(certificates_folder_path + "/certificate.crt");
		ssl_context_.use_private_key_file(certificates_folder_path + "/private.key", ssl::context::pem);
		ssl_context_.load_verify_file(certificates_folder_path + "/ca_bundle.crt");
	}
	catch (const std::exception&) {
		throw;
	}
}


ssl::context& NetworkHelper::GetSslContext() {
	return ssl_context_;
}

io_context& NetworkHelper::GetIoService() {
	return io_service_;
}

tcp::resolver::results_type NetworkHelper::ResolveHost(const std::string& host_name) {
	return tcp::resolver(io_service_).resolve(host_name, "https");
}

void NetworkHelper::Stop() {
	StopIoServiceThreads();
}


void ThreadRoutine(io_context& io_service) {
	VLDDisable();

	try {
		spdlog::get(LOG_SERVER)->trace("thread ran");
		auto handlers_counter = io_service.run();
		spdlog::get(LOG_SERVER)->trace("thread stopped, invoked handlers: {}", handlers_counter);
	}
	catch (const std::exception& err) {
		spdlog::get(LOG_SERVER)->error("io_service.run() thread exception: {}", err.what());
	}

	//clear OpenSSL environment
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
	ERR_remove_state(0);
	EVP_cleanup();

	VLDEnable();
}


void NetworkHelper::StartIoServiceThreads() {
	log->debug("create {} I/O threads", number_of_threads_);
	for (int i = 0; i < number_of_threads_; ++i)
		io_service_thread_pool.push_back(std::async(std::launch::async, ThreadRoutine, std::ref(io_service_)));
}


void NetworkHelper::StopIoServiceThreads() {
	log->debug("waiting for all threads termination");
	//io_service_work_guard_.reset();
	io_service_.stop();
	for (auto& future_object : io_service_thread_pool)	{
		if (future_object.valid()) future_object.wait();
	}
}


}  // namespace network