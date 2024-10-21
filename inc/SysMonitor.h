
#include <memory>

#include "MqttClient.h"

class SysMonitor
{
    public:
        SysMonitor(const MqttCfg &cfg);
        ~SysMonitor();

        int Initialize();
        void Deinit();
        
    private:
        MqttCfg _cfg;
        std::unique_ptr<MqttClient> _client;
};