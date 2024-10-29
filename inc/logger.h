/**
 * @file: logger.h
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief: Logging harness for the project
 * Write data to syslog or/and console
 * # View the last 10 lines of the syslog file
 * tail /var/log/syslog
 *
 * # Continuously view new entries in the syslog file
 * tail -f /var/log/syslog
 * -----
 * Copyright 2024 - KElectronics
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include <syslog.h>
#include <sstream>
#include <mutex>
#include <utility>
#include <iostream>

class Logger
{
public:
    static void Initialize(int logLevel = LOG_NOTICE, const char *ident = "logger", int facility = LOG_LOCAL1);

    template <class T>
    static void Log(std::stringstream &ss, T &&t)
    {
        ss << ' ' << std::forward<T>(t);
    }

    template <class T, class... Args>
    static void Log(std::stringstream &ss, T &&t, Args &&...args)
    {
        Log(ss, std::forward<T>(t));
        Log(ss, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void Log(int logLevel, Args &&...args)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::stringstream ss;
        Log(ss, std::forward<Args>(args)...);
        std::string logLevelStr = LogLevelToString(logLevel);
#ifdef USE_SYSLOG
        syslog(logLevel, "[%s] %s", logLevelStr.c_str(), ss.str().c_str());
#endif
#ifdef USE_STDOUT
        // printf("[%s] %s\n", logLevelStr.c_str(), ss.str().c_str());
        // change printf to cout
        std::cout << "[" << logLevelStr << "] " << ss.str() << std::endl;
#endif
    }

    template <class... Args>
    static void LogEmergency(Args &&...args)
    {
        Log(LOG_EMERG, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void LogAlert(Args &&...args)
    {
        Log(LOG_ALERT, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void LogCritical(Args &&...args)
    {
        Log(LOG_CRIT, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void LogError(Args &&...args)
    {
        Log(LOG_ERR, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void LogWarning(Args &&...args)
    {
        Log(LOG_WARNING, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void LogNotice(Args &&...args)
    {
        Log(LOG_NOTICE, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void LogInfo(Args &&...args)
    {
        Log(LOG_INFO, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void LogDebug(Args &&...args)
    {
        Log(LOG_DEBUG, std::forward<Args>(args)...);
    }

    static void Deinit();

private:
    static std::mutex logMutex;
    static std::string LogLevelToString(int logLevel);
};