#include "viber_send_strategy.h"

#include "main_config.h"


namespace handlers {
namespace output {

using boost::property_tree::ptree;
namespace http = boost::beast::http;


ViberSendStrategy::ViberSendStrategy(IClientHttpPtr client_http)
		: client_http_(client_http),
		host_(MainConfig::getInstance()->getParam("messengers.viber.http_host", std::string{ "chatapi.viber.com" })),
		target_(MainConfig::getInstance()->getParam("messengers.viber.http_target", std::string{ "/pa/send_message" })),
		token_(MainConfig::getInstance()->getParam("messengers.viber.x-viber-auth-token", std::string{ "NONE" })),
		sender_name_(MainConfig::getInstance()->getParam("messengers.viber.chat_bot_sender_name", std::string{ "tinyApps" }))
{
}


void ViberSendStrategy::Send(const std::string& user_id, const std::string& message) {
	auto http_packet{EncodeMessage(user_id, message)};
	client_http_->SendPacket(http_packet);
}


RequestStrBody ViberSendStrategy::EncodeMessage(const std::string& user_id, const std::string& message)
{
	RequestStrBody http_request;
	http_request.version(11);
	http_request.method(http::verb::post);
	http_request.target(target_);
	http_request.set(http::field::host, host_);
	http_request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	http_request.set("X-Viber-Auth-Token", token_);
	http_request.set(http::field::content_type, "application/json");
	//-------------------------------------------------------------------------------------------------------------------
	// Message JSON format:
	// {
	//   "receiver":"01234567890A=",
	//   "sender":{
	//	  "name":"John McClane"
	//   },
	//   "type":"text",
	//   "text":"Hello world!"
	// }
	//-------------------------------------------------------------------------------------------------------------------
	ptree pt;
	pt.put("receiver", user_id);
	pt.put("sender.name", sender_name_);
	pt.put("type", "text");
	pt.put("text", message);

	std::stringstream string_stream;
	boost::property_tree::write_json(string_stream, pt, false);
	http_request.body() = string_stream.str();

	http_request.prepare_payload();
	return http_request;
}


}  // namespace output
}  // namespace handlers


