#ifndef VIBER_FACTORY_H
#define VIBER_FACTORY_H

#include "i_handlers_factory.h"

//#include "main_controller.h"
#include "viber_packet_handler.h"
#include "viber_event_handler.h"
#include "viber_send_strategy.h"

namespace handlers {


/** @brief Creates and returns all kinds of Viber handlers. (Abstact Factory pattern) */
class ViberFactory : public IHandlersFactory {
public:
	ViberFactory(bot::IBotHandlerPtr chat_handler, network::client::IClientHttpPtr client_http);
	~ViberFactory() = default;

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

#endif  // VIBER_FACTORY_H