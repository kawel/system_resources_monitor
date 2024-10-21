#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

#include "SysMonitor.h"

#include "logger.h"


// std::string g_mqtt_server = "localhost";
// std::string g_mqtt_port = "1883";
// std::string g_mqtt_root_topic = "symon";

static void signalHandler(int signum);

int main(int argc, char *argv[])
{
    std::cout << "Hello, World!" << std::endl;

    Logger::Initialize(LOG_DEBUG, "sys_mon");
    Logger::LogNotice("System monitor is starting...");

    // Register signal handler
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGSEGV, signalHandler);

    MqttCfg cfg("sys_mon", "localhost", 1883, 60);
    SysMonitor system_monitor{cfg};

    if (system_monitor.Initialize() != 0)
    {
        Logger::LogError("Failed to initialize system monitor");
        return 1;
    }

    // Keep the main thread alive to allow the network loop to run

    // client.disconnect();
    Logger::Deinit();

    return 0;
}

// in case of SEGFAULT

static void signalHandler(int signum)
{
    std::cerr << "Signal " << signum << " received" << std::endl;
    Logger::Deinit();
    exit(signum);
}

