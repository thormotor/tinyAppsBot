#ifndef TELEGRAM_EVENT_HANDLER_NULL_OBJECT_H
#define TELEGRAM_EVENT_HANDLER_NULL_OBJECT_H

#include "telegram_event_handler.h"


namespace handlers {
namespace events {


/** @brief Handle all unknown Telegram events. Do nothing. (Null Object pattern / Command pattern) */
class TelegramEventHandlerNullObject : public TelegramEventHandler {
public:
	using TelegramEventHandler::TelegramEventHandler;
	~TelegramEventHandlerNullObject() = default;
	std::string Execute(const ptree& post_data) override {
		std::string event_name = post_data.get<std::string>("event", "");
		spdlog::get(LOG_MAIN)->info("not found an appropriate handler for the event \"{}\"", event_name);
		return std::string{};
	}
};


}  // namespace events
}  // namespace handlers

#endif  // TELEGRAM_EVENT_HANDLER_NULL_OBJECT_H