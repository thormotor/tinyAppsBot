#include "main_config.h"


MainConfig::MainConfig() {
	LoadConfig();
}


MainConfig* MainConfig::getInstance() {
	static MainConfig instance;
	return &instance;
}


void MainConfig::LoadConfig() {
	const char* CONFIG_FILENAME = "config.json";
	try	{
		boost::property_tree::read_json(CONFIG_FILENAME, config_);
	} catch (const std::exception& err)	{
		spdlog::get(LOG_MAIN)->critical("Error reading configuration file: {}", err.what());
	}
}


template<typename T>
T MainConfig::getParam(const std::string& param, const T& default_value) {
	T value;
	try {
		value = config_.get<T>(param);
	} catch (const std::exception& err) {
		spdlog::get(LOG_MAIN)->error("Error getting configuration parameter <{}>: \"{}\". Used default value \"{}\".", param, err.what(), default_value);
		value = default_value;
	}
	return value;
}

// explicit instantiations for getParam
template int MainConfig::getParam(const std::string&, const int&);	
template std::string MainConfig::getParam(const std::string&, const std::string&);
template bool MainConfig::getParam(const std::string&, const bool&);
