#ifndef I_REPLY_HELPER_H
#define I_REPLY_HELPER_H

#include <string>
#include <memory>

namespace handlers {
namespace output {

/** @brief Declares interface for a convenient way to reply message to a sender. (Command pattern) */
class IReplyHelper {
public:
	virtual ~IReplyHelper() = default;

	virtual void SendTextMessage(const std::string& message) const = 0;
	virtual std::string GetUserId() const = 0;
};

using IReplyHelperPtr = std::shared_ptr<IReplyHelper>;


}  // namespace output
}  // namespace handlers

#endif  // I_REPLY_HELPER_H