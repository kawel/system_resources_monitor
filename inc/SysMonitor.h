
#include <memory>
#include <vector>

#include "MqttClient.h"
#include "HwMonitor.h"
#include "Scheduler.h"

class SysMonitorTask
{
private:
    std::string _taskName;
    std::shared_ptr<IHwMonitorTask> _task;
    std::shared_ptr<IMqttClient> _client;
    std::string _topic;
    std::chrono::seconds _interval;

public:
    SysMonitorTask(const std::shared_ptr<IHwMonitorTask> &task, std::chrono::seconds interval, const std::shared_ptr<IMqttClient> &client)
        : _taskName{task->getTaskName()}, _task{task}, _client{client}, _topic{"sys_mon/data/" + _taskName}, _interval{interval} {
          };

    void run()
    {
        _task->update();
        _client->Publish(_topic, _task->dumpToJSON());
    }
};

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
    std::vector<std::shared_ptr<SysMonitorTask>> _tasks;

    std::string makeMqttTopic(const std::string &taskName) const
    {
        return _cfg._mqttRootTopic + "/" + taskName;
    }
};

