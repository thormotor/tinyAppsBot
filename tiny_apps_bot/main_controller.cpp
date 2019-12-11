#include "main_controller.h"

#include "main_config.h"
#include "network_helper.h"
#include "client_http.h"
#include "server_http.h"
#include "tiny_apps_store.h"
#include "tiny_apps_pool.h"
#include "final_packet_handler.h"
#include "viber_factory.h"
#include "telegram_factory.h"
#include "bot_handler.h"
#include "game_bulls_and_cows.h"
#include "i_reply_helper.h"


using namespace handlers;


void MainController::Start() {
	LoadMainConfig();
	InitNetworkHelper();
	InitClient();
	InitTinyAppsStore();
	InitTinyAppsPool();
	InitChatHandler();
	InitHandlerFactories();
	InitServer();
}


void MainController::Stop() {
	network_helper_->Stop();
}


void MainController::LoadMainConfig() {
	log->info("load config");
	config_ = MainConfig::getInstance();
}


void MainController::InitNetworkHelper() {
	log->info("init network environment");
	const int NUMBER_OF_THREADS_BY_DEFAULT = 10;
	const int number_of_threads = config_->getParam("network.number_of_threads", NUMBER_OF_THREADS_BY_DEFAULT);
	const std::string CERTIFICATES_PATH_BY_DEFAULT = "./certificates";
	const std::string certificates_path = config_->getParam("network.certificates_path", CERTIFICATES_PATH_BY_DEFAULT);
	network_helper_ = std::make_shared<network::NetworkHelper>(certificates_path, number_of_threads);
}


void MainController::InitHandlerFactories() {
	handler_factories_pool_.emplace_back(std::make_shared<ViberFactory>(chat_handler_, client_));
	handler_factories_pool_.emplace_back(std::make_shared<TelegramFactory>(chat_handler_, client_));
}


handlers::http::IPacketHandlerPtr MainController::InitPacketHandlersChain() {
	handlers::http::IPacketHandlerPtr first_handler = std::make_shared<http::FinalPacketHandler>();
	for (auto it = handler_factories_pool_.rbegin(); it != handler_factories_pool_.rend(); ++it) {
		(*it)->GetPacketHandler()->SetNextHandler(first_handler);
		first_handler = (*it)->GetPacketHandler();
	}
	return first_handler;
}


void MainController::InitServer() {
	const int CONNECTION_PORT_BY_DEFAULT = 443;
	const short port = config_->getParam("network.server_port", CONNECTION_PORT_BY_DEFAULT);
	log->info("init http server:{}", port);
	server_ = std::make_shared<network::server::ServerHttp>(network_helper_, port);
	server_->Subscribe(InitPacketHandlersChain());
}


void MainController::InitClient() {
	log->info("init http client");
	client_ = std::make_shared<network::client::ClientHttp>(network_helper_);
}


void MainController::InitTinyAppsStore() {
	log->info("init tiny applications storage");
	tiny_apps_store_ = std::make_shared<tiny_apps::TinyAppsStore>();

	tiny_apps::ReturnTinyAppFunction return_app_function =
		[](handlers::output::IReplyHelperPtr reply_helper) {
			return std::make_unique<tiny_apps::app_game_bulls_cows::GameBullsAndCows>(reply_helper);
		};
	tiny_apps_store_->RegisterTinyApp(return_app_function, "1", "Bulls and Cows is an old code-breaking mind game.");
}


void MainController::InitTinyAppsPool() {
	log->info("init tiny applications execution pool");
	tiny_apps_pool_ = std::make_shared<tiny_apps::TinyAppsPool>(tiny_apps_store_);
}

void MainController::InitChatHandler() {
	log->info("init bot command handler");
	chat_handler_ = std::make_shared<bot::BotHandler>(tiny_apps_pool_, tiny_apps_store_, network_helper_->GetIoService());
}
