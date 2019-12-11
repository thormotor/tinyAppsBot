#ifndef TINY_APPS_POOL_H
#define TINY_APPS_POOL_H

#include <map>
#include <mutex>
#include <memory>

#include "i_tiny_apps_pool.h"
#include "i_tiny_apps_store.h"
#include "i_tiny_app.h"
#include "i_reply_helper.h"
#include "tiny_apps_store.h"


namespace tiny_apps {


/** @brief Stores running objects of Tiny Applications. It starts and stops them. */
class TinyAppsPool : public ITinyAppsPool {
public:
	TinyAppsPool(ITinyAppsStorePtr app_store);
	~TinyAppsPool() = default;
	void StartApp(ITinyAppPtr tiny_app, const std::string& user_id) override;
	bool StopApp(const std::string& user_id) override;
	bool Turn(const std::string& user_id, const std::string& message) override;
private:
	std::map<std::string, ITinyAppPtr> tiny_apps_pool_;
	std::mutex tiny_apps_pool_mutex_;
	ITinyAppsStorePtr app_store_;
};


}  // namespace tiny_apps

#endif  // TINY_APPS_POOL_H