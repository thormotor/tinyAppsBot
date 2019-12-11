#ifndef I_BOT_HANDLER_H
#define I_BOT_HANDLER_H

#include "i_reply_helper.h"


/** @brief Contains classes for processing text messages given by bot. */
namespace bot {


/** @brief Declares interface of bot message handlers. */
class IBotHandler {
public:
	IBotHandler(const IBotHandler&) = delete;
	IBotHandler& operator=(const IBotHandler&) = delete;
	IBotHandler(IBotHandler&&) = delete;
	IBotHandler& operator=(IBotHandler&&) = delete;

	virtual void HandleChatMessage(const std::string& message, handlers::output::IReplyHelperPtr reply_helper) = 0;
protected:
	IBotHandler() = default;
	virtual ~IBotHandler() = default;
};

using IBotHandlerPtr = std::shared_ptr<IBotHandler>;


}  // namespace bot

#endif  // I_BOT_HANDLER_H