/**
 * @file: IMqttClient.h
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief: Configuration for Mqtt client
 * -----
 * Copyright 2024 - KElectronics
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include <string>
#include <MqttMsg.h>
#include <functional>

class MqttCfg
{
private:
    std::string _mqttRootTopic;

public:
    std::string _clientName;
    std::string _host;
    uint16_t _port;
    uint8_t _keepAlive;
    std::string _username;
    std::string _password;

    void setRootTopic(const std::string &rootTopic)
    {
        _mqttRootTopic = rootTopic;
        if (_mqttRootTopic.back() != '/')
        {
            _mqttRootTopic += '/';
        }
    }

    MqttCfg() = default;
    MqttCfg(const std::string &clientName, const std::string &host,
            const uint16_t &port, const uint8_t &keepAlive,
            const std::string &username = "", const std::string &password = "",
            const std::string &mqttRootTopic = "") : _mqttRootTopic(mqttRootTopic),
                                                     _clientName(clientName),
                                                     _host(host),
                                                     _port(port),
                                                     _keepAlive(keepAlive),
                                                     _username(username),
                                                     _password(password)
    {
        // check if root topic ends with '/' and add it if not
        setRootTopic(_mqttRootTopic);
    };

    std::string getRootTopic() const
    {
        return _mqttRootTopic;
    }
};

class IMqttClient
{
public:
    virtual ~IMqttClient() = default;
    virtual bool Initialize() = 0;
    virtual void Publish(const std::string &topic, const std::string &message) = 0;
    virtual void Subscribe(const std::string &topic) = 0;
    virtual void Deinit() = 0;
};