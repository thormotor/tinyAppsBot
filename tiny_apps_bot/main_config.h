#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "logger.h"


/** @brief Provides access to the main program configuration. (Singleton Mayers pattern) */
class MainConfig {
public:
	MainConfig(const MainConfig&) = delete;
	MainConfig& operator=(const MainConfig&) = delete;
	~MainConfig() = default;
	static MainConfig* getInstance();
	template <typename T = std::string>	
	T getParam(const std::string& param, const T& default_value = T{});
private:
	MainConfig();
	void LoadConfig();

	boost::property_tree::ptree config_;
};

#endif  // MAIN_CONFIG_H