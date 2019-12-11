#ifndef VIBER_EVENT_HANDLER_MESSAGE_H
#define VIBER_EVENT_HANDLER_MESSAGE_H

#include "viber_event_handler.h"


namespace handlers {
namespace events {


/** @brief Handle the "message" Viber event. Forward incoming message text and ReplyHelper object to the BotHandler. (Command pattern) */
class ViberEventHandlerMessage : public ViberEventHandler {
public:
	using ViberEventHandler::ViberEventHandler;
	~ViberEventHandlerMessage() = default;
	std::string Execute(const ptree& post_data) override {
		auto user_id = post_data.get<std::string>("sender.id", "");
		auto message_text = post_data.get<std::string>("message.text", "");
		auto reply_helper = handlers_factory_.GetReplyHelper(user_id);
		auto chat_handler = handlers_factory_.GetChatHandler();
		chat_handler->HandleChatMessage(message_text, reply_helper);
		return std::string{};
	}
};


}  // namespace events
}  // namespace handlers

#endif  // VIBER_EVENT_HANDLER_MESSAGE_H