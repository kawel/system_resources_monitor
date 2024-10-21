// logger.h
#pragma once

#include <mutex>
#include <sstream>
#include <syslog.h>
#include <utility>

#define EMERG LOG_EMERG
#define ALERT LOG_ALERT
#define CRIT LOG_CRIT
#define ERR LOG_ERR
#define WARN LOG_WARNING
#define NOTICE LOG_NOTICE
#define INFO LOG_INFO
#define DEBUG LOG_DEBUG

class Logger {
public:
    static void Initialize(int logLevel = LOG_NOTICE, const char* ident = "logger", int facility = LOG_LOCAL1);
    template <class T>
    static void Log(std::stringstream& ss, T&& t);
    template <class T, class... Args>
    static void Log(std::stringstream& ss, T&& t, Args&&... args);
    template <class... Args>
    static void Log(int logLevel, Args&&... args);
    static void Deinit();

private:
    static std::mutex logMutex;
    static std::string LogLevelToString(int logLevel);
};