#include "bot_handler.h"

#include <boost/algorithm/string.hpp>

#include "logger.h"


namespace bot {

using handlers::output::IReplyHelperPtr;


BotHandler::BotHandler(tiny_apps::ITinyAppsPoolPtr tiny_apps_pool, tiny_apps::ITinyAppsStorePtr tiny_apps_store, boost::asio::io_context& multithreaded_context)
		: tiny_apps_pool_{ tiny_apps_pool },
		tiny_apps_store_{ tiny_apps_store },
		multithreaded_context_{ multithreaded_context } 
{
	InitCommandHandlers();
}

void BotHandler::HandleChatMessage(const std::string& message, IReplyHelperPtr reply_helper) {
	std::string message_removed_line_feeds;
	std::remove_copy(message.begin(), message.end(), std::back_inserter(message_removed_line_feeds), '\n');
	spdlog::get(LOG_MAIN)->info("<<< {}: \"{}\"", reply_helper->GetUserId().substr(0,6).append("___"), message_removed_line_feeds);

	// handle incoming bot message in another thread
	boost::asio::post(multithreaded_context_, [=]() {
		Execute(message, reply_helper);
	});
}


void BotHandler::InitCommandHandlers() {
	command_handlers_.emplace("stop", &BotHandler::CommandStop);
	command_handlers_.emplace("app", &BotHandler::CommandStart);
	command_handlers_.emplace("list", &BotHandler::CommandList);
	command_handlers_.emplace("?", &BotHandler::CommandHelp);
}


void BotHandler::Execute(const std::string& message, IReplyHelperPtr reply_helper) {
	std::vector<std::string> splitted_message;
	boost::split(splitted_message, boost::to_lower_copy(message), boost::is_space(), boost::token_compress_on);
	try	{
		CommandMethodPtr handler_ptr = command_handlers_.at(splitted_message.at(0));
		(this->*handler_ptr)(splitted_message, reply_helper);
	} catch (const std::out_of_range&) {
		if (!tiny_apps_pool_->Turn(reply_helper->GetUserId(), message)) {
			reply_helper->SendTextMessage("\"" + message + "\"" + u8" - unknown command. \nSend me '?' for help.\u00af\\_(\u30c4)_/\u00af");
		}
	}
}


void BotHandler::CommandStop(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper) {
	spdlog::get(LOG_MAIN)->debug("BotHandler::CommandStop()");
	if (tiny_apps_pool_->StopApp(reply_helper->GetUserId())) {
		reply_helper->SendTextMessage("Tiny application is stopped.");
	} else {
		reply_helper->SendTextMessage("No started tiny applications!");
	}
}


void BotHandler::CommandStart(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper) {
	spdlog::get(LOG_MAIN)->debug("BotHandler::CommandStart()");
	if (splitted_message.size() >= 2) {
		std::string app_name = splitted_message.at(1);
		auto tiny_app = tiny_apps_store_->GetApplication(app_name, reply_helper);
		if (tiny_app) {
			tiny_apps_pool_->StartApp(std::move(tiny_app), reply_helper->GetUserId());
		} else {
			reply_helper->SendTextMessage(splitted_message.at(1) + " - unknown tiny application. Use 'list' to see available.");
		}
	} else {
		reply_helper->SendTextMessage("Use 'app [number]'.\nFor example: 'app 1'.");
	}
}


void BotHandler::CommandList(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper) {
	std::string list_of_apps_description;
	list_of_apps_description += "You could run any application from the list below.\n\n";
	for (const auto& tiny_app : tiny_apps_store_->GetApplicationsList()) {
		list_of_apps_description += "'" + tiny_app.first + "'  -  ";
		list_of_apps_description += tiny_app.second + "\n\n";
	}
	list_of_apps_description += "*Send me, for example, 'app 1' to run the new bulls&cows game.";
	reply_helper->SendTextMessage(list_of_apps_description);
}


void BotHandler::CommandHelp(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper) {
	spdlog::get(LOG_MAIN)->debug("BotHandler::CommandHelp()");
	std::string help_message;
	help_message += "*TinyAppsBot* is a tiny applications bot. Use 'list' to see the available tiny applications. You could choose one from the list and run it. \nAll other commands:\n\n";
	help_message += "'?'  -  this help info;\n";
	help_message += "'app [N]'  -  start tiny app;\n";
	help_message += "'stop'  -  stop curent tiny app;\n";
	help_message += "'list'  -  list available tiny apps;";

	reply_helper->SendTextMessage(help_message);
}


}  // namespace bot
