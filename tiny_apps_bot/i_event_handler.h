#ifndef I_EVENT_HANDLER_H
#define I_EVENT_HANDLER_H

#include <string>

#include "boost/property_tree/ptree.hpp"


namespace handlers {
/** @brief Contains handlers for the different type of incoming message events. */
namespace events {


/** @brief Declares interface of event handlers. (Command pattern) */
class IEventHandler {
public:
	IEventHandler(const IEventHandler&) = delete;
	IEventHandler& operator=(const IEventHandler&) = delete;
	IEventHandler(IEventHandler&&) = delete;
	IEventHandler& operator=(IEventHandler&&) = delete;

	virtual std::string Execute(const boost::property_tree::ptree& post_data) = 0;
protected:
	IEventHandler() = default;
	virtual ~IEventHandler() = default;
};

using IEventHandlerPtr = std::shared_ptr<IEventHandler>;


}  // namespace events
}  // namespace handlers

#endif  // I_EVENT_HANDLER_H