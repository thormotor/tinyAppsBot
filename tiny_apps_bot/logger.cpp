#include "logger.h"

// definition global names of available loggers
const char* LOG_MAIN = "main__";
const char* LOG_SERVER = "server";
const char* LOG_CLIENT = "client";
const char* LOG_DATA = "data__";


namespace logger {


// initialize logging evironment
void LoggerInit() {

	// define log filenames
	const char* APP_LOG_FILENAME = "_app.log";
	const char* DATA_LOG_FILENAME = "_data.log";

	// define log files rotation parameters
	constexpr size_t ROTATION_FILE_SIZE = 1024 * 1024 * 5;  // 5 Mbytes
	constexpr size_t ROTATION_FILE_COUNT = 3;  // 3 files

	// define output format string
	const char* OUTPUT_FORMAT = "[%Y-%m-%d %T] %^[%L]%$ [%n] %v {th:%t}";

	using spdlog::details::registry;
	using namespace spdlog::level;
	using namespace spdlog::sinks;

#ifdef WIN32
	auto console_sink = std::make_shared<wincolor_stdout_sink_mt>();
#else
	auto console_sink = std::make_shared<ansicolor_stdout_sink_mt>();
#endif // WIN32

	auto file_sink_app = std::make_shared<rotating_file_sink_mt>(APP_LOG_FILENAME, ROTATION_FILE_SIZE, ROTATION_FILE_COUNT);
	auto file_sink_data = std::make_shared<rotating_file_sink_mt>(DATA_LOG_FILENAME, ROTATION_FILE_SIZE, ROTATION_FILE_COUNT);

	auto log_main = registry::instance().create(LOG_MAIN, { console_sink, file_sink_app });
	auto log_server = registry::instance().create(LOG_SERVER, { console_sink, file_sink_app });
	auto log_client = registry::instance().create(LOG_CLIENT, { console_sink, file_sink_app });
	auto log_data = registry::instance().create(LOG_DATA, { file_sink_data });

	log_main->set_pattern(OUTPUT_FORMAT);
	log_server->set_pattern(OUTPUT_FORMAT);
	log_client->set_pattern(OUTPUT_FORMAT);
	log_data->set_pattern(OUTPUT_FORMAT);

	spdlog::set_level(trace);
	console_sink->set_level(info);
	file_sink_app->set_level(trace);
	file_sink_data->set_level(trace);

}


}  // namespace logger
