/**
* @file: MqttClient.h
* @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
* @brief: 
* -----
* Copyright 2024 - KElectronics
* -----
* HISTORY:
* Date      	By	Comments
* ----------	---	---------------------------------------------------------
*/

#pragma once

#include "IMqttClient.h"

#include "logger.h"
#include "mosquitto.h"

class MqttClient : public IMqttClient
{
public:
    MqttClient() : _clientId{}, _host{}, _port{}, _keepAlive{}, _mosq{nullptr} {};
    MqttClient(const MqttCfg &cfg) : MqttClient(cfg._clientName, cfg._host, cfg._port, cfg._keepAlive) {};
    MqttClient(const std::string &clientId, const std::string &host, int port, int keepAlive = 60);
    ~MqttClient();

    bool Initialize()  override;
    void Deinit() override;
    void Publish(const std::string &topic, const std::string &message) override;
    void Subscribe(const std::string &topic) override;

private:
    bool connect();
    void disconnect();
    void loop();
    // add queue for messages


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
            Logger::LogNotice("...broker disconnected");
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