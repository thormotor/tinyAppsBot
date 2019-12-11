#ifndef TELEGRAM_SEND_STRATEGY_H
#define TELEGRAM_SEND_STRATEGY_H

#include <boost/beast.hpp>

#include "i_send_strategy.h"
#include "client_http.h"


namespace handlers {
namespace output {

using network::client::IClientHttpPtr;
using RequestStrBody = boost::beast::http::request<boost::beast::http::string_body>;
using ResponseStrBody = boost::beast::http::response<boost::beast::http::string_body>;


/** @brief Prepares text message HTTP packet for Telegram and passes them to http_client. (Strategy pattern) */
class TelegramSendStrategy : public ISendStrategy {
 public:
	TelegramSendStrategy(IClientHttpPtr client_http);
	~TelegramSendStrategy() = default;
	void Send(const std::string& user_id, const std::string& message) override;
 private:
	RequestStrBody EncodeMessage(const std::string& user_id, const std::string& message);

	IClientHttpPtr client_http_;
	const std::string host_;
	const std::string target_prefix_;
	const std::string target_method_;
	const std::string token_;
};


}  // namespace output
}  // namespace handlers

#endif  // TELEGRAM_SEND_STRATEGY_H