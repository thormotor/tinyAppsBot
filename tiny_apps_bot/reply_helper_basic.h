#ifndef REPLY_HELPER_BASIC_H
#define REPLY_HELPER_BASIC_H

#include "i_reply_helper.h"
#include "i_send_strategy.h"

namespace handlers {
namespace output {


/** @brief Implements IReplyHelper interface for replying to the sender. It knows whom and how it should send a message. (Command pattern) */
class ReplyHelperBasic : public IReplyHelper {
public:
	ReplyHelperBasic(const std::string& user_id, ISendStrategyPtr send_strategy);
	~ReplyHelperBasic() = default;

	void SendTextMessage(const std::string& message) const override;
	std::string GetUserId() const override;
private:
	std::string user_id_;
	ISendStrategyPtr send_strategy_;
};


}  // namespace output
}  // namespace handlers

#endif  // REPLY_HELPER_BASIC_H