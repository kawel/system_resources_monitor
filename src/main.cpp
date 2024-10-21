#include <iostream>

#include "logger.h"

#include "MqttClient.h"
#include <thread>
#include <chrono>

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

    try {
        MqttClient client("test_client", "localhost", 1883);
        client.connect();
        client.subscribe("test/topic");
        client.publish("test/topic", "Hello, MQTT!");
        client.loop();

        // Keep the main thread alive to allow the network loop to run
        std::this_thread::sleep_for(std::chrono::seconds(10));
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }


    Logger::Deinit();
    return 0;
}
