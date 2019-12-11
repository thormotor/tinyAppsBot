#ifndef VIBER_EVENT_HANDLER_DELIVERED_H
#define VIBER_EVENT_HANDLER_DELIVERED_H

#include "viber_event_handler.h"


namespace handlers {
namespace events {


/** @brief Handle the "delivered" Viber event. Do nothing. (Command pattern) */
class ViberEventHandlerDelivered : public ViberEventHandler {
public:
	using ViberEventHandler::ViberEventHandler;
	~ViberEventHandlerDelivered() = default;
	std::string Execute(const ptree& post_data) override {
		spdlog::get(LOG_MAIN)->debug("received status message \"delivered\"");
		return std::string{};
	}
};


}  // namespace events
}  // namespace handlers

#endif  // VIBER_EVENT_HANDLER_DELIVERED_H