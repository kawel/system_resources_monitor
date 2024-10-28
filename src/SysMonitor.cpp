#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "logger.h"
#include "SysMonitor.h"

SysMonitor::SysMonitor(const MqttCfg &cfg)
    : _cfg{cfg},
      _taskScheduler{},
      _hwMonitor{},
      _client{nullptr},
      _topic{"sys_mon/data/"},
      _tasks{}
{

    _taskScheduler.addTask([this]()
                           {
        this->_hwMonitor.getUpTimeInfo().update();
        this->_client->Publish(_topic + this->_hwMonitor.getUpTimeInfo().getTaskName(), 
        this->_hwMonitor.getUpTimeInfo().dumpToJSON()); },
                           std::chrono::seconds(10));

    _taskScheduler.addTask([this]()
                           {
        this->_hwMonitor.getLoadAvg().update();
        this->_client->Publish(_topic + this->_hwMonitor.getLoadAvg().getTaskName(),
        this->_hwMonitor.getLoadAvg().dumpToJSON()); },
                           std::chrono::seconds(2));

    _taskScheduler.addTask([this]()
                           {
        this->_hwMonitor.getMemInfo().update();
        this->_client->Publish(_topic + this->_hwMonitor.getMemInfo().getTaskName(),
        this->_hwMonitor.getMemInfo().dumpToJSON()); },
                           std::chrono::seconds(2));

    _taskScheduler.addTask([this]()
                           {
        this->_hwMonitor.getVersionInfo().update();
        this->_client->Publish(_topic + this->_hwMonitor.getVersionInfo().getTaskName(),
        this->_hwMonitor.getVersionInfo().dumpToJSON()); },
                           std::chrono::seconds(15));

    for (const auto &ipLink : _hwMonitor.getIpLinkStatistics())
    {
        _taskScheduler.addTask([this, ipLink]()
                               {
            ipLink->update();
            this->_client->Publish(_topic + ipLink->getTaskName(), ipLink->dumpToJSON()); },
                               std::chrono::seconds(5));
    }
}

SysMonitor::~SysMonitor()
{
}

int SysMonitor::Initialize()
{
    try
    {
        _client = std::make_unique<MqttClient>(_cfg);
        _client->Initialize();
        _client->Subscribe("test/topic");
        _client->Publish("test/topic", "Hello, MQTT!");

        // Keep the main thread alive to allow the network loop to run
        std::this_thread::sleep_for(std::chrono::seconds(2));
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
    _client->Deinit();
    _client.reset();
}