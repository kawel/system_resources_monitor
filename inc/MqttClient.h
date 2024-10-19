#pragma once

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

class MosquittoManager : public IMqttManager
{
public:
   MosquittoManager(const MqttCfg &);
   bool Initialize() override;
   void Publish(const MqttMsg::MqttTopicPub &, uint8_t *buffer, std::size_t size) override;
   void Subscribe(const MqttMsg::MqttTopicSub &, std::function<void(MqttMsg::MqttTopicSub, uint8_t *, std::size_t)>) override;
   void Deinit() override;

protected:
   MqttCfg _cfg;
   mosquitto *_mosq{nullptr};
};