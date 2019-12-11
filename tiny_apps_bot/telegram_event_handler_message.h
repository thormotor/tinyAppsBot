#ifndef TELEGRAM_EVENT_HANDLER_MESSAGE_H
#define TELEGRAM_EVENT_HANDLER_MESSAGE_H

#include "telegram_event_handler.h"


namespace handlers {
namespace events {


/** @brief Handle the "message" Viber event. Forward message text and ReplyHelper object to BotHandler. (Command pattern) */
class TelegramEventHandlerMessage : public TelegramEventHandler {
public:
	using TelegramEventHandler::TelegramEventHandler;
	~TelegramEventHandlerMessage() = default;
	std::string Execute(const ptree& post_data) override {
		auto user_id = post_data.get<std::string>("message.from.id", "");
		auto message_text = post_data.get<std::string>("message.text", "");
		auto reply_helper = handlers_factory_.GetReplyHelper(user_id);
		auto chat_handler = handlers_factory_.GetChatHandler();
		chat_handler->HandleChatMessage(message_text, reply_helper);
		return std::string{};
	}
};


}  // namespace events
}  // namespace handlers

#endif  // TELEGRAM_EVENT_HANDLER_MESSAGE_H