#ifndef I_TINY_APPS_STORE_H
#define I_TINY_APPS_STORE_H

#include <memory>
#include <map>
#include <functional>

#include "i_tiny_app.h"
#include "i_reply_helper.h"


/** @brief Contains classes for supporting tiny applications */
namespace tiny_apps {

using ReturnTinyAppFunction = std::function<ITinyAppPtr(handlers::output::IReplyHelperPtr)>;


/** @brief Declares interface of TinyApplications factory that returns objects by request. */
class ITinyAppsStore {
public:
	ITinyAppsStore(const ITinyAppsStore&) = delete;
	ITinyAppsStore& operator=(const ITinyAppsStore&) = delete;
	ITinyAppsStore(ITinyAppsStore&&) = delete;
	ITinyAppsStore& operator=(ITinyAppsStore&&) = delete;

	virtual const std::map<std::string, std::string> GetApplicationsList() = 0;
	virtual ITinyAppPtr GetApplication(const std::string& app_name, handlers::output::IReplyHelperPtr reply_helper) = 0;
	virtual void RegisterTinyApp(ReturnTinyAppFunction tiny_app_creator, const std::string& start_name, const std::string& description) = 0;
protected:
	ITinyAppsStore() = default;
	virtual ~ITinyAppsStore() = default;
};

using ITinyAppsStorePtr = std::shared_ptr<ITinyAppsStore>;


}  // namespace tiny_apps

#endif  // I_TINY_APPS_STORE_H