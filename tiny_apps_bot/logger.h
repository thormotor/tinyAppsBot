#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"


// declaration of global names of available loggers
extern const char* LOG_MAIN;  // usage: auto log = spdlog::get(LOG_MAIN);
extern const char* LOG_SERVER;
extern const char* LOG_CLIENT;
extern const char* LOG_DATA;

namespace logger {

void LoggerInit();

}  // namespace logger

#endif  // LOGGER_H