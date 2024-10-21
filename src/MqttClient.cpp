#include <mosquitto.h>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <chrono>

#include "MqttClient.h"
#include "logger.h"

MqttClient::MqttClient(const std::string &clientId, const std::string &host, int port, int keepAlive)
    : _clientId(clientId), _host(host), _port(port), _keepAlive(keepAlive)
{
    Logger::LogDebug("Creating Mosquitto instance...");
    mosquitto_lib_init();
    _mosq = mosquitto_new(_clientId.c_str(), true, this);
    if (!_mosq)
    {
        Logger::LogError("Failed to create Mosquitto instance");
        throw std::runtime_error("Failed to create Mosquitto instance");
    }

    mosquitto_connect_callback_set(_mosq, onConnect);
    mosquitto_message_callback_set(_mosq, onMessage);
    mosquitto_disconnect_callback_set(_mosq, onDisconnect);
}

MqttClient::MqttClient(const MqttCfg &cfg)
    : MqttClient(cfg._clientName, cfg._host, cfg._port, cfg._keepAlive)
{
}

MqttClient::~MqttClient()
{
    mosquitto_disconnect(_mosq);
    mosquitto_loop_stop(_mosq, true);
    mosquitto_destroy(_mosq);
    mosquitto_lib_cleanup();
    Logger::LogDebug("Mosquitto instance destroyed");
}

void MqttClient::Initialize()
{
    if (!connect())
    {
        throw std::runtime_error("Failed to connect to broker");
    }

    loop();
}

void MqttClient::Deinit()
{
    disconnect();
}

bool MqttClient::connect()
{
    int res = mosquitto_connect(_mosq, _host.c_str(), _port, _keepAlive);
    if (res != MOSQ_ERR_SUCCESS)
    {
        throw std::runtime_error("Failed to connect to broker");
        Logger::LogError("Failed to connect to broker:", mosquitto_strerror(res));
        return false;
    }
    Logger::LogInfo("Connecting to broker:", _host, ":", _port);
    return true;
}

void MqttClient::disconnect()
{
    mosquitto_disconnect(_mosq);
    // 1s delau to allow the network loop to run
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mosquitto_loop_stop(_mosq, true);
}

void MqttClient::loop()
{
    Logger::LogDebug("Starting Mosquitto loop");
    mosquitto_loop_start(_mosq);
}

void MqttClient::Publish(const std::string &topic, const std::string &message)
{
    mosquitto_publish(_mosq, nullptr, topic.c_str(), message.size(), message.c_str(), 0, false);
}

void MqttClient::Subscribe(const std::string &topic)
{
    mosquitto_subscribe(_mosq, nullptr, topic.c_str(), 0);
}
