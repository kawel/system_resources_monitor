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

void task1() {
    std::cout << "Task 1 running every 1 second" << std::endl;
}

void task3() {
    std::cout << "Task 3 running every 3 seconds" << std::endl;
}

void task5() {
    std::cout << "Task 5 running every 5 seconds" << std::endl;
}

int main() {
    TaskScheduler scheduler;

    scheduler.addTask(task1, std::chrono::seconds(1));
    scheduler.addTask(task3, std::chrono::seconds(3));
    scheduler.addTask(task5, std::chrono::seconds(5));

    scheduler.start();

    std::this_thread::sleep_for(std::chrono::seconds(20)); // Run for 20 seconds

    scheduler.stop();

    return 0;
}