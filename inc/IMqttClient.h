#pragma once

#include <string>
#include <MqttMsg.h>
#include <functional>

class IMqttManager
{
public:
   IMqttManager(){}
   virtual ~IMqttManager(){}
   virtual bool Initialize() = 0;
   virtual void Publish(const MqttMsg::MqttTopicPub&, uint8_t* buffer, std::size_t size) = 0;
   virtual void Subscribe(const MqttMsg::MqttTopicSub&, std::function<void(MqttMsg::MqttTopicSub, uint8_t*, std::size_t)>) = 0;
   virtual void Deinit() = 0;
};