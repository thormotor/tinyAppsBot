#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include <vector>

#include "logger.h"
#include "main_config.h"
#include "i_tiny_apps_store.h"
#include "i_tiny_apps_pool.h"
#include "i_network_helper.h"
#include "i_client_http.h"
#include "i_server_http.h"
#include "i_send_strategy.h"
#include "i_bot_handler.h"
#include "i_handlers_factory.h"
#include "i_reply_helper.h"
#include "i_packet_handler.h"


/** @brief Creates and manages others objects. Maintains starting and finishing the program. */
class MainController {
public:
	MainController() = default;
	MainController(const MainController&) = delete;
	MainController& operator=(const MainController&) = delete;
	MainController(MainController&&) = delete;
	MainController& operator=(MainController&&) = delete;
	~MainController() = default;

	void Start();
	void Stop();
private:
	void LoadMainConfig();
	void InitNetworkHelper();
	void InitHandlerFactories();
	handlers::http::IPacketHandlerPtr InitPacketHandlersChain();
	void InitServer();
	void InitClient();
	void InitTinyAppsStore();
	void InitTinyAppsPool();
	void InitChatHandler();

	MainConfig* config_ = nullptr;
	network::INetworkHelperPtr network_helper_;
	network::client::IClientHttpPtr client_;
	network::server::IServerHttpPtr server_;
	tiny_apps::ITinyAppsStorePtr tiny_apps_store_;
	tiny_apps::ITinyAppsPoolPtr tiny_apps_pool_;
	bot::IBotHandlerPtr chat_handler_;
	std::vector<handlers::IHandlersFactoryPtr> handler_factories_pool_;
	std::shared_ptr<spdlog::logger> log = spdlog::get(LOG_MAIN);
};

#endif  // MAIN_CONTROLLER_H