#ifndef I_SEND_STRATEGY_H
#define I_SEND_STRATEGY_H

#include <string>
#include <memory>


namespace handlers {
/** @brief Contains handlers for outgoing messages */
namespace output {


/** @brief Declares interface of outgoing message handlers. (Strategy pattern) */
class ISendStrategy {	
public:
	ISendStrategy() = default;
	virtual ~ISendStrategy() = default;
	virtual void Send(const std::string& user_id, const std::string& message) = 0;
};

using ISendStrategyPtr = std::shared_ptr<ISendStrategy>;


}  // namespace output
}  // namespace handlers

#endif  // I_SEND_STRATEGY_H