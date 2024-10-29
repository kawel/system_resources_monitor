#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "logger.h"
#include "SysMonitor.h"


void SysMonitor::scheduleTask(IHwMonitorTask &task, std::chrono::seconds interval) {
    _taskScheduler.addTask([&task, this]() {
        task.update();
        _client->Publish(_topic + task.getTaskName(), task.dumpToJSON());
    }, interval);
}

SysMonitor::SysMonitor(const MqttCfg &cfg)
    : _cfg{cfg},
      _taskScheduler{},
      _hwMonitor{},
      _client{nullptr},
      _topic{cfg.getRootTopic()}
{
    // Schedule tasks using the template function
    scheduleTask(_hwMonitor.getUpTimeInfo(), std::chrono::seconds(1));
    scheduleTask(_hwMonitor.getLoadAvg(), std::chrono::seconds(10));
    scheduleTask(_hwMonitor.getVersionInfo(), std::chrono::seconds(60));
    scheduleTask(_hwMonitor.getMemInfo(), std::chrono::seconds(2));

    for (const auto &ipLinkStatistics : _hwMonitor.getIpLinkStatistics())
    {
        scheduleTask(*ipLinkStatistics, std::chrono::seconds(5));
    }
}

SysMonitor::~SysMonitor()
{
    Logger::LogDebug("SysMonitor destructor");
    Stop();
    Deinit();
}

int SysMonitor::Initialize()
{
    try
    {
        _client = std::make_unique<MqttClient>(_cfg);
        _client->Initialize();
        _client->Subscribe("test/topic");
        _client->Publish("test/topic", "Hello, MQTT!");
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

void SysMonitor::Deinit()
{
    Logger::LogDebug("SysMonitor deinit");
    _client->Deinit();
    _client.reset();
}