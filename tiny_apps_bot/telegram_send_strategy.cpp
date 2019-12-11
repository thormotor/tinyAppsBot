#include "telegram_send_strategy.h"
#include "main_config.h"


namespace handlers {
namespace output {

using boost::property_tree::ptree;
namespace http = boost::beast::http;


TelegramSendStrategy::TelegramSendStrategy(IClientHttpPtr client_http)
		: client_http_(client_http),
		host_(MainConfig::getInstance()->getParam("messengers.telegram.http_host", std::string{ "api.telegram.org" })),
		target_prefix_(MainConfig::getInstance()->getParam("messengers.telegram.http_target_prefix", std::string{ "/bot" })),
		target_method_(MainConfig::getInstance()->getParam("messengers.telegram.http_target_method_send_message", std::string{ "/sendMessage" })),
		token_(MainConfig::getInstance()->getParam("messengers.telegram.token", std::string{ "NONE" }))
{
}


void TelegramSendStrategy::Send(const std::string& user_id, const std::string& message) {
	auto http_packet{ EncodeMessage(user_id, message) };
	client_http_->SendPacket(http_packet);
}


RequestStrBody TelegramSendStrategy::EncodeMessage(const std::string& user_id, const std::string& message)
{
	RequestStrBody http_request;
	http_request.version(11);
	http_request.method(http::verb::post);
	http_request.target(target_prefix_ + token_ + target_method_);
	http_request.set(http::field::host, host_);
	http_request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	http_request.set(http::field::content_type, "application/json");

	ptree pt;
	pt.put("chat_id", user_id);
	pt.put("text", message);

	std::stringstream string_stream;
	boost::property_tree::write_json(string_stream, pt, false);
	http_request.body() = string_stream.str();

	http_request.prepare_payload();
	return http_request;
}


}  // namespace output
}  // namespace handlers


