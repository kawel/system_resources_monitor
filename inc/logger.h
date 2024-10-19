/*
# View the last 10 lines of the syslog file
tail /var/log/syslog

# Continuously view new entries in the syslog file
tail -f /var/log/syslog
*/

#include <syslog.h>
#include <sstream>
#include <mutex>
#include <utility>

#define EMERG LOG_EMERG
#define ALERT LOG_ALERT
#define CRIT LOG_CRIT
#define ERR LOG_ERR
#define WARN LOG_WARNING
#define NOTICE LOG_NOTICE
#define INFO LOG_INFO
#define DEBUG LOG_DEBUG

class Logger
{
public:
    static void Initialize(int logLevel = LOG_NOTICE, const std::string& ident = "symon", int facility = LOG_LOCAL1)
    {
        setlogmask(LOG_UPTO(logLevel));
        openlog(ident.c_str(), LOG_PID | LOG_NDELAY, facility);
    }

    template <class T>
    static void Log(std::stringstream& ss, T&& t)
    {
        ss << ' ' << std::forward<T>(t);
    }

    template <class T, class... Args>
    static void Log(std::stringstream& ss, T&& t, Args&&... args)
    {
        Log(ss, std::forward<T>(t));
        Log(ss, std::forward<Args>(args)...);
    }

    template <class... Args>
    static void Log(int logLevel, Args&&... args)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::stringstream ss;
        Log(ss, std::forward<Args>(args)...);
        std::string logLevelStr = LogLevelToString(logLevel);
        // syslog(logLevel, "[%s] %s", logLevelStr.c_str(), ss.str().c_str());
        printf("[%s] %s\n", logLevelStr.c_str(), ss.str().c_str());
    }

    static void Deinit()
    {
        closelog();
    }

private:
    static std::mutex logMutex;

    static std::string LogLevelToString(int logLevel)
    {
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
};

// Define the static mutex
std::mutex Logger::logMutex;