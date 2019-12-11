#include "tiny_apps_pool.h"

#include "logger.h"


namespace tiny_apps {


TinyAppsPool::TinyAppsPool(ITinyAppsStorePtr app_store)
		: app_store_{app_store}
{
}


void TinyAppsPool::StartApp(ITinyAppPtr tiny_app, const std::string& user_id) {
	std::lock_guard<std::mutex> guard(tiny_apps_pool_mutex_);
	tiny_apps_pool_[user_id] = std::move(tiny_app);
}

bool TinyAppsPool::StopApp(const std::string & user_id) {
	std::lock_guard<std::mutex> guard(tiny_apps_pool_mutex_);
	return tiny_apps_pool_.erase(user_id) > 0;
}

bool TinyAppsPool::Turn(const std::string & user_id, const std::string & message) {
	std::lock_guard<std::mutex> guard(tiny_apps_pool_mutex_);

	auto search_it = tiny_apps_pool_.find(user_id);
	if (search_it != tiny_apps_pool_.end()) {
		search_it->second->Turn(message);
		return true;
	} else {
		return false;
	}
}


}  // namespace tiny_apps

