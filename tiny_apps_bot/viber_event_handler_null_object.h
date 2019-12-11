#ifndef VIBER_EVENT_HANDLER_NULL_OBJECT_H
#define VIBER_EVENT_HANDLER_NULL_OBJECT_H

#include "viber_event_handler.h"


namespace handlers {
namespace events {


/** @brief Handle all unknown Viber events. Do nothing. (Null Object pattern / Command pattern) */
class ViberEventHandlerNullObject : public ViberEventHandler {
public:
	using ViberEventHandler::ViberEventHandler;
	~ViberEventHandlerNullObject() = default;
	std::string Execute(const ptree& post_data) override {
		std::string event_name = post_data.get<std::string>("event", "");
		spdlog::get(LOG_MAIN)->info("not found an appropriate handler for the event \"{}\"", event_name);
		return std::string{};
	}
};


}  // namespace events
}  // namespace handlers

#endif  // VIBER_EVENT_HANDLER_NULL_OBJECT_H