#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <memory>

#include "SysMonitor.h"

#include "logger.h"

const MqttCfg g_cfg("sys_mon", "localhost", 1883, 60);
static std::unique_ptr<SysMonitor> g_system_monitor;

static void signalHandler(int signum);

int main(int argc, char *argv[])
{
    Logger::Initialize(LOG_DEBUG, g_cfg._clientName.c_str());
    Logger::LogNotice("System monitor is starting...");

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

    Logger::Deinit();

    return 0;
}

// TODO: Implement signal handlers for SIGINT, SIGTERM, and SIGSEGV
static void signalHandler(int signum)
{
    std::cerr << "Signal " << signum << " received" << std::endl;
    Logger::Deinit();
    exit(signum);
}
