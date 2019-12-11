#ifndef BOT_HANDLER_H
#define BOT_HANDLER_H

#include <map>
#include <memory>
#include <vector>
#include <boost/asio.hpp>

#include "i_bot_handler.h"
#include "i_tiny_apps_pool.h"
#include "i_tiny_apps_store.h"
#include "i_reply_helper.h"

#include <functional>

namespace bot {

using handlers::output::IReplyHelperPtr;

class BotHandler;

using CommandMethodPtr = void(BotHandler::*)(const std::vector<std::string>&, IReplyHelperPtr);
using CommandFunction = std::function<void(const std::vector<std::string>&, IReplyHelperPtr)>;


/** @brief Parses input message from bot and calls appropriate command handler. */
class BotHandler : public IBotHandler {
public:
	BotHandler(tiny_apps::ITinyAppsPoolPtr tiny_apps_pool, tiny_apps::ITinyAppsStorePtr tiny_apps_store, boost::asio::io_context& multithreaded_context);
	~BotHandler() = default;
	void HandleChatMessage(const std::string& message, IReplyHelperPtr reply_helper) override;
private:
	void Execute(const std::string& message, IReplyHelperPtr reply_helper);
	// --- command handlers initializer ---
	void InitCommandHandlers();
	// --- command handlers methods ---
	void CommandStop(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper);
	void CommandStart(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper);
	void CommandList(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper);
	void CommandHelp(const std::vector<std::string>& splitted_message, IReplyHelperPtr reply_helper);

	std::map<std::string, CommandMethodPtr> command_handlers_;
	tiny_apps::ITinyAppsPoolPtr tiny_apps_pool_;
	tiny_apps::ITinyAppsStorePtr tiny_apps_store_;
	boost::asio::io_context& multithreaded_context_;
};


}  // namespace chat_intelligense

#endif  // BOT_HANDLER_H