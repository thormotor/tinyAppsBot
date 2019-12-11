#ifndef VIBER_EVENT_HANDLER_SEEN_H
#define VIBER_EVENT_HANDLER_SEEN_H

#include "viber_event_handler.h"


namespace handlers {
namespace events {


/** @brief Handle the "seen" Viber event. Do nothing. (Command pattern) */
class ViberEventHandlerSeen : public ViberEventHandler {
public:
	using ViberEventHandler::ViberEventHandler;
	~ViberEventHandlerSeen() = default;
	std::string Execute(const ptree& post_data) override {
		spdlog::get(LOG_MAIN)->debug("received status message \"seen\"");
		return std::string{};
	}
};


}  // namespace events
}  // namespace handlers

#endif  // VIBER_EVENT_HANDLER_SEEN_H