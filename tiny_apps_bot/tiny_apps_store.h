#ifndef TINY_APPS_STORE_H
#define TINY_APPS_STORE_H

#include <map>
#include <memory>

#include "i_tiny_apps_store.h"
#include "i_reply_helper.h"


namespace tiny_apps {

using handlers::output::IReplyHelperPtr;


/** @brief Stores list of available Tiny Applications and creates by request their objects. */
class TinyAppsStore : public ITinyAppsStore {
public:
	TinyAppsStore() = default;
	~TinyAppsStore() = default;
	const std::map<std::string, std::string> GetApplicationsList() override;
	ITinyAppPtr GetApplication(const std::string& app_name, IReplyHelperPtr reply_helper) override;
	void RegisterTinyApp(ReturnTinyAppFunction tiny_app_creator, const std::string& start_name, const std::string& description) override;
private:
	struct TinyAppData {
		ReturnTinyAppFunction tiny_app_creator;
		std::string start_name;
		std::string description;
	};

	std::map<std::string, TinyAppData> tiny_apps_list_;
};


}  // namespace tiny_apps

#endif  // TINY_APPS_STORE_H