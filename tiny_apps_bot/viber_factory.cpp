#include "viber_factory.h"

#include <functional>

#include "viber_event_handler_null_object.h"
#include "viber_event_handler_message.h"
#include "viber_event_handler_started.h"
#include "viber_event_handler_delivered.h"
#include "viber_event_handler_seen.h"
#include "viber_send_strategy.h"
#include "reply_helper_basic.h"


namespace handlers {


ViberFactory::ViberFactory(bot::IBotHandlerPtr chat_handler, network::client::IClientHttpPtr client_http)
		: chat_handler_(chat_handler)
{
	InitPacketHandler();
	InitSendStrategy(client_http);
	InitEventHandlers();
}


void ViberFactory::InitPacketHandler() {
	packet_handler_ = std::make_shared<http::ViberPacketHandler>(this);
}

void ViberFactory::InitEventHandlers() {
	event_handlers_.emplace("message", std::make_shared<events::ViberEventHandlerMessage>(*this));
	event_handlers_.emplace("conversation_started", std::make_shared<events::ViberEventHandlerStarted>(*this));
	event_handlers_.emplace("delivered", std::make_shared<events::ViberEventHandlerDelivered>(*this));
	event_handlers_.emplace("seen", std::make_shared<events::ViberEventHandlerSeen>(*this));
}


void ViberFactory::InitSendStrategy(network::client::IClientHttpPtr client_http) {
	send_strategy_ = std::make_shared<output::ViberSendStrategy>(client_http);
}


http::IPacketHandlerPtr ViberFactory::GetPacketHandler() {
	return packet_handler_;
}


events::IEventHandlerPtr ViberFactory::GetEventHandler(const std::string& event_name) {
	auto it = event_handlers_.find(event_name);
	if (it != event_handlers_.end()) {
		return it->second;
	} else {
		return std::make_shared<events::ViberEventHandlerNullObject>(*this);
	}
}


output::ISendStrategyPtr ViberFactory::GetSendStrategy() {
	return send_strategy_;
}


output::IReplyHelperPtr ViberFactory::GetReplyHelper(const std::string& user_id) {
	return std::make_shared<output::ReplyHelperBasic>(user_id, send_strategy_);
}


bot::IBotHandlerPtr ViberFactory::GetChatHandler() {
	return chat_handler_;
}


}  // namespace handlers