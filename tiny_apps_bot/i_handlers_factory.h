#ifndef I_HANDLERS_FACTORY_H
#define I_HANDLERS_FACTORY_H

#include <memory>
#include <functional>
#include <map>

#include "i_send_strategy.h"
#include "i_event_handler.h"
#include "i_packet_handler.h"
#include "i_reply_helper.h"
#include "i_bot_handler.h"


/** @brief Contains handlers that support HTTP, JSON and Messengers APIs. */
namespace handlers {

using AppHandlerFunction = std::function<void(const std::string&, output::IReplyHelperPtr)>;


/**
@brief Declares factory interface for a set of handlers related to different kinds of messengers. (Abstract Factory pattern)

Produce handlers and helpers to processing incoming packets for different kind of messengers.
*/
class IHandlersFactory {
public:
	IHandlersFactory(const IHandlersFactory&) = delete;
	IHandlersFactory& operator=(const IHandlersFactory&) = delete;
	IHandlersFactory(IHandlersFactory&&) = delete;
	IHandlersFactory& operator=(IHandlersFactory&&) = delete;

	virtual http::IPacketHandlerPtr GetPacketHandler() = 0;
	virtual events::IEventHandlerPtr GetEventHandler(const std::string& event_name) = 0;
	virtual output::ISendStrategyPtr GetSendStrategy() = 0;
	virtual output::IReplyHelperPtr GetReplyHelper(const std::string& user_id) = 0;
	virtual bot::IBotHandlerPtr GetChatHandler() = 0;
protected:
	IHandlersFactory() = default;
	virtual ~IHandlersFactory() = default;
};

using IHandlersFactoryPtr = std::shared_ptr<IHandlersFactory>;


}  // namespace handlers

#endif  // I_HANDLERS_FACTORY_H