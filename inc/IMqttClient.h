#pragma once

#include <string>
#include <MqttMsg.h>
#include <functional>

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


class IMqttClient
{
public:
   IMqttClient(){}
   virtual ~IMqttClient(){}
   virtual bool Initialize() = 0;
   virtual void Publish(const std::string &topic, const std::string &message) = 0;
   virtual void Subscribe(const std::string &topic) = 0;
   virtual void Deinit() = 0;
};