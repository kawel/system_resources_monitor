#pragma once

#include "logger.h"

#include <IMqttClient.h>
#include "mosquitto.h"

struct MqttCfg
{
    std::string _clientName;
    std::string _host;
    uint16_t _port;
    uint8_t _keepAlive;

    MqttCfg(const std::string &clientName, const std::string &host, const uint16_t &port, const uint8_t &keepAlive)
        : _clientName(clientName),
          _host(host),
          _port(port),
          _keepAlive(keepAlive)
    {
    }
};

class MqttClient
{
public:
    MqttClient() : _clientId{}, _host{}, _port{}, _keepAlive{}, _mosq{nullptr} {};
    MqttClient(const MqttCfg &cfg);
    MqttClient(const std::string &clientId, const std::string &host, int port, int keepAlive = 60);
    ~MqttClient();

    bool connect();
    void disconnect();
    void loop();
    void publish(const std::string &topic, const std::string &message);
    void subscribe(const std::string &topic);

private:
    static void onConnect(struct mosquitto *mosq, void *obj, int result)
    {
        if (result == 0)
        {
            std::cout << "Connected to broker" << std::endl;
        }
        else
        {
            std::cerr << "Failed to connect to broker: " << mosquitto_strerror(result) << std::endl;
        }
    }

    static void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
    {
        std::cout << "Received message: " << (char *)msg->payload << " on topic: " << msg->topic << std::endl;
    }

    std::string _clientId;
    std::string _host;
    int _port;
    int _keepAlive;
    struct mosquitto *_mosq;
};