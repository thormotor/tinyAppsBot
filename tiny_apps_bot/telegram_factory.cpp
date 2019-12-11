#include "telegram_factory.h"

#include <functional>

#include "telegram_event_handler_null_object.h"
#include "telegram_event_handler_message.h"
#include "telegram_send_strategy.h"
#include "reply_helper_basic.h"



namespace handlers {

TelegramFactory::TelegramFactory(bot::IBotHandlerPtr chat_handler, network::client::IClientHttpPtr client_http)
		: chat_handler_(chat_handler)
{
	InitPacketHandler();
	InitSendStrategy(client_http);
	InitEventHandlers();
}


void TelegramFactory::InitPacketHandler() {
	packet_handler_ = std::make_shared<http::TelegramPacketHandler>(this);
}

void TelegramFactory::InitEventHandlers() {
	event_handlers_.emplace("message", std::make_shared<events::TelegramEventHandlerMessage>(*this));
}


void TelegramFactory::InitSendStrategy(network::client::IClientHttpPtr client_http) {
	send_strategy_ = std::make_shared<output::TelegramSendStrategy>(client_http);
}


http::IPacketHandlerPtr TelegramFactory::GetPacketHandler() {
	return packet_handler_;
}


events::IEventHandlerPtr TelegramFactory::GetEventHandler(const std::string& event_name) {
	auto it = event_handlers_.find(event_name);
	if (it != event_handlers_.end()) {
		return it->second;
	} else {
		return std::make_shared<events::TelegramEventHandlerNullObject>(*this);
	}
}


output::ISendStrategyPtr TelegramFactory::GetSendStrategy() {
	return send_strategy_;
}


output::IReplyHelperPtr TelegramFactory::GetReplyHelper(const std::string& user_id) {
	return std::make_shared<output::ReplyHelperBasic>(user_id, send_strategy_);
}

bot::IBotHandlerPtr TelegramFactory::GetChatHandler() {
	return chat_handler_;
}


}  // namespace handlers