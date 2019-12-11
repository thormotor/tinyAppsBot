#ifndef VIBER_EVENT_HANDLER_H
#define VIBER_EVENT_HANDLER_H

#include "i_event_handler.h"
#include "i_handlers_factory.h"
#include "logger.h"


namespace handlers {
namespace events {

using boost::property_tree::ptree;


/** @brief Used as a base class for all Viber event handlers. Keeps the reference to the Viber handlers factory. (Command pattern) */
class ViberEventHandler : public IEventHandler {
public:
	ViberEventHandler(IHandlersFactory& handlers_factory) 
			: handlers_factory_(handlers_factory) {
	}
	~ViberEventHandler() = default;
	virtual std::string Execute(const ptree& post_data) = 0;
protected:
	 IHandlersFactory& handlers_factory_;
};


}  // namespace events
}  // namespace handlers

#endif  // VIBER_EVENT_HANDLER_H