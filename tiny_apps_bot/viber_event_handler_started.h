#ifndef VIBER_EVENT_HANDLER_STARTED_H
#define VIBER_EVENT_HANDLER_STARTED_H

//#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "viber_event_handler.h"



namespace handlers {
namespace events {


/** @brief Handle the "conversation_started" Viber event. Replies to sender a welcome message. (Command pattern) */
class ViberEventHandlerStarted : public ViberEventHandler {
public:
	using ViberEventHandler::ViberEventHandler;
	~ViberEventHandlerStarted() = default;
	std::string Execute(const ptree& post_data) override {
		auto user_id = post_data.get<std::string>("user.id", "");
		auto user_name = post_data.get<std::string>("user.name", "");
		auto is_subscribed = post_data.get<bool>("subscribed", false);
		spdlog::get(LOG_MAIN)->info("<<< Conversation started with {}: \"{}\" ({})", user_id.substr(0, 6).append("___"), user_name, (is_subscribed ? "subscribed": "unsubscribed"));
		auto response_body = std::string{};
		if (!is_subscribed) {
			ptree pt;
			pt.put("sender.name", "tinyAppsBot");
			pt.put("type", "text");
			pt.put("text", "Welcome to *tinyAppsBot* bot!\nSend me '?' to get help. :)");
			std::stringstream string_stream;
			boost::property_tree::write_json(string_stream, pt, false);
			response_body = string_stream.str();
		}
		return response_body;
	}
};


}  // namespace events
}  // namespace handlers

#endif  // VIBER_EVENT_HANDLER_STARTED_H
