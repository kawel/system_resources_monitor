#include <iostream>

#include "logger.h"

// std::string g_mqtt_server = "localhost";
// std::string g_mqtt_port = "1883";
// std::string g_mqtt_root_topic = "symon";

int main(int argc, char *argv[])
{
    std::cout << "Hello, World!" << std::endl;

    Logger::Initialize(LOG_DEBUG, "symon");
    Logger::Log(LOG_NOTICE, "System monitor is starting...");
    Logger::LogDebug("Debug message");
    Logger::LogInfo("Info message");
    Logger::LogNotice("Notice message");
    Logger::LogWarning("Warning message");
    Logger::LogError("Error message");
    Logger::LogCritical("Critical message");
    Logger::LogAlert("Alert message");
    Logger::LogEmergency("Emergency message");

    Logger::Deinit();
    return 0;
}
