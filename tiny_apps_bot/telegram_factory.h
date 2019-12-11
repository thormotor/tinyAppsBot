#ifndef TELEGRAM_FACTORY_H
#define TELEGRAM_FACTORY_H

#include "i_handlers_factory.h"

#include "telegram_packet_handler.h"
#include "telegram_event_handler.h"
#include "telegram_send_strategy.h"

namespace handlers {


/** @brief Creates and returns all kinds of Telegram handlers. (Abstact Factory pattern) */
class TelegramFactory : public IHandlersFactory {
public:
	TelegramFactory(bot::IBotHandlerPtr chat_handler, network::client::IClientHttpPtr client_http);
	~TelegramFactory() = default;

	http::IPacketHandlerPtr GetPacketHandler() override;
	events::IEventHandlerPtr GetEventHandler(const std::string& event_name) override;
	output::ISendStrategyPtr GetSendStrategy() override;
	output::IReplyHelperPtr GetReplyHelper(const std::string& user_id) override;
	bot::IBotHandlerPtr GetChatHandler() override;
private:
	void InitPacketHandler();
	void InitEventHandlers();
	void InitSendStrategy(network::client::IClientHttpPtr client_http);

	//MainController* main_controller_;
	http::IPacketHandlerPtr packet_handler_;
	std::map<std::string, events::IEventHandlerPtr> event_handlers_;
	output::ISendStrategyPtr send_strategy_;
	bot::IBotHandlerPtr chat_handler_;
};


}  //namespace handlers

#endif  // TELEGRAM_FACTORY_H