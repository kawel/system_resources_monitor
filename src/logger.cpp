// logger.cpp
#include "logger.h"

std::mutex Logger::logMutex;

void Logger::Initialize(int logLevel, const char* ident, int facility) {
    setlogmask(LOG_UPTO(logLevel));
    openlog(ident, LOG_PID | LOG_NDELAY, facility);
    #ifdef USE_SYSLOG
        printf("Logger is using syslog\n");
    #else
        printf("Logger is using printf\n");
    #endif
}


void Logger::Deinit() {
    closelog();
}

std::string Logger::LogLevelToString(int logLevel) {
    switch (logLevel) {
        case LOG_EMERG:   return "EMERG";
        case LOG_ALERT:   return "ALERT";
        case LOG_CRIT:    return "CRIT";
        case LOG_ERR:     return "ERR";
        case LOG_WARNING: return "WARN";
        case LOG_NOTICE:  return "NOTICE";
        case LOG_INFO:    return "INFO";
        case LOG_DEBUG:   return "DEBUG";
        default:          return "UNKNOWN";
    }
}
