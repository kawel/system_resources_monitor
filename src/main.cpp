#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <memory>
#include <execinfo.h> // For backtrace

#include "SysMonitor.h"
#include "GetOptions.h"
#include "logger.h"

#define VERSION "1.1.2"

static MqttCfg g_cfg("sys_mon", "localhost", 1883, 60, "user", "password", "sys_mon/data");
static std::unique_ptr<SysMonitor> g_system_monitor;

static void signalHandler(int signum);

int main(int argc, char *argv[])
{
    Logger::LogNotice("System monitor version: ", VERSION);
    Logger::LogNotice("Build date: ", __DATE__, " ", __TIME__);

    switch (getOptions(g_cfg, argc, argv))
    {
    case 0:
        break;
    case 1:
        return 0;
    default:
        Logger::LogError("Failed to get options");
        return -1;
    }

    Logger::Initialize(LOG_DEBUG, g_cfg._clientName.c_str());

    // Register signal handler
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGSEGV, signalHandler);

    g_system_monitor = std::make_unique<SysMonitor>(g_cfg);
    if (g_system_monitor->Initialize() != 0)
    {
        Logger::LogError("Failed to initialize system monitor");
        return 1;
    }

    g_system_monitor->Start();

    // loop forever
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

static void signalHandler(int signum)
{
    if (signum == SIGSEGV)
    {
        Logger::LogError("Segmentation fault (signal %d) received.", signum);

        // Print backtrace
        void *array[10];
        size_t size = backtrace(array, 10);
        char **messages = backtrace_symbols(array, size);
        Logger::LogError("Backtrace:");
        for (size_t i = 0; i < size; i++)
        {
            Logger::LogError("%s", messages[i]);
        }
        free(messages);

        // Minimal cleanup
        if (g_system_monitor)
        {
            g_system_monitor->Stop();
            g_system_monitor->Deinit();
        }

        Logger::Deinit();
        exit(signum);
    }
    else
    {
        Logger::LogNotice("Signal %d received", signum);
        Logger::LogNotice("Stopping system monitor...");

        if (g_system_monitor)
        {
            g_system_monitor->Stop();
            g_system_monitor->Deinit();
        }

        Logger::Deinit();
        exit(signum);
    }
}
