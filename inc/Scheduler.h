#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <atomic>

class TaskScheduler {
public:
    TaskScheduler() : running(true) {}

    ~TaskScheduler() {
        stop();
    }

    void addTask(std::function<void()> task, std::chrono::milliseconds interval) {
        tasks.emplace_back([this, task, interval]() {
            while (running) {
                std::this_thread::sleep_for(interval);
                if (running) task();
            }
        });
    }

    void start() {
        for (auto& task : tasks) {
            threads.emplace_back(std::thread(task));
        }
    }

    void stop() {
        running = false;
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
};