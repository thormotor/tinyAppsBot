#ifndef TELEGRAM_EVENT_HANDLER_H
#define TELEGRAM_EVENT_HANDLER_H

#include "i_event_handler.h"
#include "i_handlers_factory.h"
#include "logger.h"


namespace handlers {
namespace events {

using boost::property_tree::ptree;


/** @brief Used as a base class for all Telegram event handlers. Keeps the reference to the Telegram handlers factory. (Command pattern) */
class TelegramEventHandler : public IEventHandler {
public:
	TelegramEventHandler(IHandlersFactory& handlers_factory) 
			: handlers_factory_(handlers_factory) {
	}
	~TelegramEventHandler() = default;
	virtual std::string Execute(const ptree& post_data) = 0;
protected:
	IHandlersFactory& handlers_factory_;
};


}  // namespace events
}  // namespace handlers

#endif  // TELEGRAM_EVENT_HANDLER_H