#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "logger.h"

namespace sys_monitor {

class TaskScheduler {
public:
    TaskScheduler() : running(true) {}

    ~TaskScheduler() {
        stop();
    }

    void addTask(std::function<void()> task, std::chrono::milliseconds interval) {
        tasks.emplace_back([this, task, interval]() {
            while (running) {
                std::unique_lock<std::mutex> lock(mutex);
                if (cv.wait_for(lock, interval, [this]() { return !running; })) {
                    break;
                }
                task();
            }
        });
    }

    void start() {
        for (auto& task : tasks) {
            threads.emplace_back(std::thread(task));
        }
    }

    void stop() {
        Logger::LogDebug("Stopping task scheduler");
        {
            std::lock_guard<std::mutex> lock(mutex);
            running = false;
        }
        cv.notify_all();
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

private:
    std::vector<std::function<void()>> tasks;
    std::vector<std::thread> threads;
    std::atomic<bool> running;
    std::condition_variable cv;
    std::mutex mutex;
};

} // namespace sys_monitor