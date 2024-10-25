#include "Scheduler.h"
#include "gtest/gtest.h"


TEST(Scheduler, FunctionalTest)
{
    TaskScheduler scheduler;

    scheduler.addTask([]() { std::cout << "Task 1 running every 1 second" << std::endl; }, std::chrono::seconds(1));
    scheduler.addTask([]() { std::cout << "Task 3 running every 3 seconds" << std::endl; }, std::chrono::seconds(3));
    scheduler.addTask([]() { std::cout << "Task 5 running every 5 seconds" << std::endl; }, std::chrono::seconds(5));

    scheduler.start();

    std::this_thread::sleep_for(std::chrono::seconds(10)); // Run for 20 seconds

    scheduler.stop();
}