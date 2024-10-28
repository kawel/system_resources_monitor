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
    // for (const auto &task : _hwMonitor.getTasks())
    // {
    //     _tasks.push_back(std::make_shared<SysMonitorTask>(task, std::chrono::seconds(5), _client));
    // }
    int n =1;
    for (const auto &task : _hwMonitor.getTasks())
    {
        _taskScheduler.addTask([this, task]() {
            task->update();
            _client->Publish(_topic + task->getTaskName(), task->dumpToJSON());
        }, std::chrono::seconds(n++));
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