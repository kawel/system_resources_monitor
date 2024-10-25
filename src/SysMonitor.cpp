#include <iostream>
#include <thread>
#include <chrono>

#include "logger.h"
#include "SysMonitor.h"


SysMonitor::SysMonitor(const MqttCfg &cfg)
    : _cfg{cfg},
    _taskScheduler{}, 
    // _hwMonitor{}, 
    _client{nullptr}
{
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

    // PeriodicTask<UpTimeInfo> *periodicTask;
    // for (auto &task : _hwMonitor.getTasks())
    // {
        // periodicTask = dynamic_cast<PeriodicTask<UpTimeInfo> *>(task.get());
        // Logger::Log(LOG_NOTICE, "Task: ", periodicTask->getTask().getTaskName());
        // _taskScheduler.addTask([this, &task]() {
        //     Logger::Log(LOG_NOTICE, "Task: ", task->serialize());
        //     _client->Publish("test/topic", task->serialize());
        // }, std::chrono::seconds{task->getPeriod()});
        // _taskScheduler.addTask(task.getTask().run(), std::chrono::seconds{task->getPeriod()});
    // }


    return 0;
}

void SysMonitor::Deinit()
{
    _client->Deinit();
    _client.reset();
}