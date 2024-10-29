#pragma once

#include <cstdint>
#include <string>

namespace MqttMsg
{
   struct Msg
   {
      std::string topic;
      int qos;

      Msg(const std::string& topic, const int& qos)
         : topic(topic),
           qos(qos)
      {
      }
   };

   enum class MqttTopicPub
   {
      cv2x_bsm = 0,
      cv2x_latency,
      nmea_sentence,
      thermal_status,
      pdop,
      hdop,
      vdop
   };

   enum class MqttTopicSub
   {
      antenna_id = 0,
      simulation_enabled,
      simulation_file,
      can,
      unsupported
   };

//    std::size_t createBasicSafetyMessage(const uint64_t& stationId, const uint32_t& latitude, const uint32_t& longitude,
//                                         const uint32_t& altitude, const uint16_t& speed, const CanData& can, uint8_t* buffer);

//    std::size_t createPdopMessage(const float& pdop, uint8_t* buffer);
//    std::size_t createHdopMessage(const float& hdop, uint8_t* buffer);
//    std::size_t createVdopMessage(const float& vdop, uint8_t* buffer);
//    std::size_t createLatencyMessage(const uint64_t& stationId, const uint16_t& latency, uint8_t* buffer);

//    std::string getTopicStringPub(const MqttTopicPub&);
//    std::string getTopicStringSub(const MqttTopicSub&);
//    MqttTopicSub getTopicSubFromString(const std::string&);
};
