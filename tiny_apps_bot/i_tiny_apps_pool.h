#ifndef I_TINY_APPS_POOL_H
#define I_TINY_APPS_POOL_H

#include <memory>

#include "i_tiny_app.h"


namespace tiny_apps {


/** @brief Declares interface of running tiny applications pool. */
class ITinyAppsPool {
public:
	ITinyAppsPool(const ITinyAppsPool&) = delete;
	ITinyAppsPool& operator=(const ITinyAppsPool&) = delete;
	ITinyAppsPool(ITinyAppsPool&&) = delete;
	ITinyAppsPool& operator=(ITinyAppsPool&&) = delete;

	virtual void StartApp(ITinyAppPtr tiny_app, const std::string& user_id) = 0;
	virtual bool StopApp(const std::string& user_id) = 0;
	virtual bool Turn(const std::string& user_id, const std::string& message) = 0;
protected:
	ITinyAppsPool() = default;
	virtual ~ITinyAppsPool() = default;
};

using ITinyAppsPoolPtr = std::shared_ptr<ITinyAppsPool>;


}  // namespace tiny_apps

#endif  // I_TINY_APPS_POOL_H