// logger.cpp
#include "logger.h"

std::mutex Logger::logMutex;

void Logger::Initialize(int logLevel, const char* ident, int facility) {
    setlogmask(LOG_UPTO(logLevel));
    openlog(ident, LOG_PID | LOG_NDELAY, facility);
}

template <class T>
void Logger::Log(std::stringstream& ss, T&& t) {
    ss << ' ' << std::forward<T>(t);
}

template <class T, class... Args>
void Logger::Log(std::stringstream& ss, T&& t, Args&&... args) {
    Log(ss, std::forward<T>(t));
    Log(ss, std::forward<Args>(args)...);
}

template <class... Args>
void Logger::Log(int logLevel, Args&&... args) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::stringstream ss;
    Log(ss, std::forward<Args>(args)...);
    std::string logLevelStr = LogLevelToString(logLevel);
    printf("LogLevel %d: [%s] %s\n", logLevel, logLevelStr.c_str(), ss.str().c_str());
    // syslog(logLevel, "[%s] %s", logLevelStr.c_str(), ss.str().c_str());
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

// Explicit instantiation of template methods
template void Logger::Log(std::stringstream& ss, const char*&& t);
template void Logger::Log(std::stringstream& ss, const char*&& t, const char*&& t2);
template void Logger::Log(int logLevel, const char*&& t);
template void Logger::Log(int logLevel, const char*&& t, const char*&& t2);