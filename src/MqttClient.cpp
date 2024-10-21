#include <mosquitto.h>
#include <iostream>
#include <cstring>
#include <stdexcept>

#include "MqttClient.h"
// #include "logger.h"

MqttClient::MqttClient(const std::string &clientId, const std::string &host, int port)
    : _clientId(clientId), _host(host), _port(port)
{
    mosquitto_lib_init();
    _mosq = mosquitto_new(_clientId.c_str(), true, this);
    if (!_mosq)
    {
        throw std::runtime_error("Failed to create Mosquitto instance");
        // Logger::Log(ERROR, "Failed to create Mosquitto instance");
    }

    mosquitto_connect_callback_set(_mosq, onConnect);
    mosquitto_message_callback_set(_mosq, onMessage);
}

MqttClient::~MqttClient()
{
    mosquitto_destroy(_mosq);
    mosquitto_lib_cleanup();
}

bool MqttClient::connect()
{
    int res = mosquitto_connect(_mosq, _host.c_str(), _port, 60);
    if (res != MOSQ_ERR_SUCCESS)
    {
        throw std::runtime_error("Failed to connect to broker");
        return false;
    }
    return true;
}

void MqttClient::loop()
{
    mosquitto_loop_start(_mosq);
}

void MqttClient::publish(const std::string &topic, const std::string &message)
{
    mosquitto_publish(_mosq, nullptr, topic.c_str(), message.size(), message.c_str(), 0, false);
}

void MqttClient::subscribe(const std::string &topic)
{
    mosquitto_subscribe(_mosq, nullptr, topic.c_str(), 0);
}
