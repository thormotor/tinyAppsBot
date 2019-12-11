#include "reply_helper_basic.h"

#include "logger.h"


namespace handlers {
namespace output {


ReplyHelperBasic::ReplyHelperBasic(const std::string& user_id, ISendStrategyPtr send_strategy) 
		: user_id_(user_id), send_strategy_(send_strategy) 
{
}	


void ReplyHelperBasic::SendTextMessage(const std::string& message) const {
	std::string removed_line_feeds;
	std::remove_copy(message.begin(), message.end(), std::back_inserter(removed_line_feeds), '\n');
	spdlog::get(LOG_MAIN)->info(">>> {}: \"{}\"", user_id_.substr(0, 6).append("___"), removed_line_feeds);
	send_strategy_->Send(user_id_, message);
}


std::string ReplyHelperBasic::GetUserId() const {
	return user_id_;
}


}  // namespace output
}  // namespace handlers