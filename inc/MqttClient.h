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


class MqttClient {
public:
   MqttClient(const std::string &clientId, const std::string &host, int port);
   ~MqttClient();

   bool connect();
   void disconnect();
   void loop();
   void publish(const std::string &topic, const std::string &message);
   void subscribe(const std::string &topic);
    // static void onMessage(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);

private:
    static void onConnect(struct mosquitto* mosq, void* obj, int result) {
        if (result == 0) {
            std::cout << "Connected to broker" << std::endl;
        } else {
            std::cerr << "Failed to connect to broker: " << mosquitto_strerror(result) << std::endl;
        }
    }

    static void onMessage(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {
        std::cout << "Received message: " << (char*)msg->payload << " on topic: " << msg->topic << std::endl;
    }

    std::string _clientId;
    std::string _host;
    int _port;
    struct mosquitto* _mosq;
};