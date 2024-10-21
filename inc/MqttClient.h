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

    void Initialize();
    void Deinit();
    void Publish(const std::string &topic, const std::string &message);
    void Subscribe(const std::string &topic);

private:
    bool connect();
    void disconnect();
    void loop();

    static void onConnect(struct mosquitto *mosq, void *obj, int result)
    {
        if (result == 0)
        {
            Logger::LogNotice("...broker connected");
            static_cast<MqttClient *>(obj)->_connected = true;
        }
        else
        {
            Logger::LogError("Failed to connect to broker:", mosquitto_strerror(result));
            static_cast<MqttClient *>(obj)->_connected = false;
        }
    }

    static void onDisconnect(struct mosquitto *mosq, void *obj, int result)
    {
        if (result == 0)
        {
            Logger::LogWarning("...broker disconnected");
            static_cast<MqttClient *>(obj)->_connected = false;
        }
        else
        {
            Logger::LogError("Failed to disconnect from broker:", mosquitto_strerror(result));
        }
    }

    static void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
    {
        Logger::LogDebug("Received message: ", (char *)msg->payload, " on topic: ", msg->topic);
    }

    std::string _clientId;
    std::string _host;
    int _port;
    int _keepAlive;
    struct mosquitto *_mosq;

protected:
    bool _connected = false;
};