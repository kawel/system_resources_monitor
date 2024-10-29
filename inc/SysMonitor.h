
#include <vector>
#include <string>
#include <chrono>

#include "MqttClient.h"
#include "HwMonitor.h"
#include "Scheduler.h"
#include "IHwMonitor.h"


class SysMonitor
{
public:
    SysMonitor(const MqttCfg &cfg);
    ~SysMonitor();

    int Initialize();
    void Deinit();
    void Start() { _taskScheduler.start(); }
    void Stop() { _taskScheduler.stop(); }
    
private:
    MqttCfg _cfg;
    sys_monitor::TaskScheduler _taskScheduler;
    HwMonitor _hwMonitor;
    std::unique_ptr<IMqttClient> _client;
    std::string _topic;

    void scheduleTask(IHwMonitorTask &task, std::chrono::seconds interval);
};
