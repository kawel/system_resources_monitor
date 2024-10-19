#include <MqttClient.h>
#include <iostream>
#include <logger.h>
#include <MqttMsg.h>
#include <functional>
#include <map>
#include <chrono>
#include <thread>

namespace
{
//    std::map<MqttMessage::MqttTopicSub, std::function<void(MqttMessage::MqttTopicSub, uint8_t*, std::size_t)>> _actionForTopic;

   void OnMessage(mosquitto *mosq, void *obj, const struct mosquitto_message* message)
   {
      Logger::Log(DEBUG, "Received MQTT message ", message->topic);
    //   auto topic =  MqttMessage::getTopicSubFromString(message->topic);

    //   if(topic == MqttMessage::MqttTopicSub::unsupported)
    //   {
    //      Logger::Log(INFO, "Unsupported topic ", message->topic);
    //      return;
    //   }

    //   auto iter = _actionForTopic.find(topic);
    //   if(iter != _actionForTopic.end())
    //   {
    //      auto topic = (*iter).first;
    //      auto action = (*iter).second;
    //      action(topic, (uint8_t*)message->payload, message->payloadlen);
    //   }
    //   else
    //   {
    //      Logger::Log(INFO, "Cannot find action for ", message->topic);
    //   }
   }
}

MosquittoManager::MosquittoManager(const MqttCfg& cfg) : _cfg(cfg)
{
}

bool MosquittoManager::Initialize()
{
   auto res = mosquitto_lib_init();
   if(res != MOSQ_ERR_SUCCESS)
   {
      Logger::Log(ERR, "Cannot initialize mosquittto lib!");
      return false;
   }
   _mosq = mosquitto_new(_cfg._clientName.c_str(), true, nullptr);
   if(_mosq == nullptr)
   {
      Logger::Log(INFO, "Cannot initialize mosquittto!");
      return false;
   }

   mosquitto_message_callback_set(_mosq, OnMessage);
   res = MOSQ_ERR_NO_CONN;

   while (res != MOSQ_ERR_SUCCESS)
   {
      res = mosquitto_connect(_mosq, _cfg._host.c_str(), _cfg._port, _cfg._keepAlive);
      if(res != MOSQ_ERR_SUCCESS)
      {
         Logger::Log("Cannot connect to mosquittto broker! Error Code:", res, " Retry after 5 seconds...");
         std::this_thread::sleep_for(std::chrono::milliseconds(5000));
      }
   }

   mosquitto_loop_start(_mosq);
   return true;
}

void MosquittoManager::Publish(const MqttMessage::MqttTopicPub& topic, uint8_t* buffer, std::size_t size)
{
   auto topicStr = MqttMessage::getTopicStringPub(topic);
   if(topicStr.empty())
   {
      Logger::Log("Cannot find topic");
      return;
   }
   auto res = mosquitto_publish(_mosq, nullptr, topicStr.c_str(), size, buffer, 1, false);
}

void MosquittoManager::Subscribe(const MqttMessage::MqttTopicSub& topic, std::function<void(MqttMessage::MqttTopicSub, uint8_t*, std::size_t)> action)
{
   auto topicStr = MqttMessage::getTopicStringSub(topic);
   if(topicStr.empty())
   {
      Logger::Log("Cannot find topic");
      return;
   }

   _actionForTopic.insert({topic, action});
   mosquitto_subscribe(_mosq, nullptr, topicStr.c_str(), 0);
}

void MosquittoManager::Deinit()
{
   mosquitto_loop_stop(_mosq, true);
   mosquitto_lib_cleanup();
}