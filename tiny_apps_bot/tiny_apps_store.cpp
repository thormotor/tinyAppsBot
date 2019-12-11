#include "tiny_apps_store.h"

#include <algorithm>

#include "game_bulls_and_cows.h"


namespace tiny_apps {


const std::map<std::string, std::string> TinyAppsStore::GetApplicationsList() {
	std::map<std::string, std::string> available_tiny_apps_list;
	std::for_each(tiny_apps_list_.begin(), tiny_apps_list_.end(), 
		[&](auto& app_data) { 
			available_tiny_apps_list.emplace(app_data.second.start_name, app_data.second.description); 
		});
	return available_tiny_apps_list;
}


ITinyAppPtr TinyAppsStore::GetApplication(const std::string& app_name, IReplyHelperPtr reply_helper) {
	try {
		auto tiny_app_data = tiny_apps_list_.at(app_name);
		return std::move(tiny_app_data.tiny_app_creator(reply_helper));
	} catch (const std::out_of_range&) {
		return ITinyAppPtr();  // return NULL pointer
	}
}

void TinyAppsStore::RegisterTinyApp(ReturnTinyAppFunction tiny_app_creator, const std::string& start_name, const std::string& description) {
	tiny_apps_list_.emplace(start_name, TinyAppData{ tiny_app_creator, start_name, description });
}


}  // namespace tiny_apps