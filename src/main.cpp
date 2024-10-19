#include <iostream>

#include "logger.h"

// std::string g_mqtt_server = "localhost";
// std::string g_mqtt_port = "1883";
// std::string g_mqtt_root_topic = "symon";

int main(int argc, char* argv[])
{
    std::cout << "Hello, World!" << std::endl;
    Logger::Initialize(LOG_NOTICE, "symon");
    Logger::Log(NOTICE, "System monitor is starting...");

    Logger::Deinit();
    return 0;
}
