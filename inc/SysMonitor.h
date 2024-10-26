
#include <memory>

#include "MqttClient.h"
#include "HwMonitor.h"
#include "Scheduler.h"

class SysMonitor
{
public:
    SysMonitor(const MqttCfg &cfg);
    ~SysMonitor();

    int Initialize();
    void Deinit();

private:
    MqttCfg _cfg;
    sys_monitor::TaskScheduler _taskScheduler;
    HwMonitor _hwMonitor;
    std::unique_ptr<IMqttClient> _client;
};