#include <syslog.h>
#include <string>
#include <sstream>

class Logger
{
public:
   static void Initialize()
   {
      setlogmask(LOG_UPTO (LOG_NOTICE));
      openlog("symon", LOG_PID | LOG_NDELAY, LOG_LOCAL1);
   }

   template <class T>
   static void Log(std::stringstream& ss, T&& t)
   {
      ss << ' ' << t;
   }

   template <class T, class... Args>
   static void Log(std::stringstream& ss, T&& t, Args&&... args)
   {
      Log(ss, t);
      Log(ss, args...);
   }

   template <class... Args>
   static void Log(Args&&... args)
   {
      std::stringstream ss;
      Log(ss, args...);
      syslog(LOG_NOTICE, "%s", ss.str().c_str());
   }

   static void Deinit()
   {
       closelog();
   }
};