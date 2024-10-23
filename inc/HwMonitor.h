/**
* @file: HwMonitor.h
* @brief: 
* -----
* Copyright 2024 - KElectronics
* -----
* HISTORY:
* Date      	By	Comments
* ----------	---	---------------------------------------------------------
*/

#pragma once

#include <string>

// class HwMonitor
// {
// private:
//     /* data */
//     int up_time;
// public:
//     HwMonitor(/* args */);
//     ~HwMonitor();
//     int getUpTime();
//     int getTemp();
//     int getRamUsage();
//     int getCpuUsage();
//     int getDiskUsage();
//     int getNetUsage();
//     int getNetSpeed();
     
// };

class UpTimeInfo
{
private:
    double _uptime;
    std::string _uptimeFilePath;

public:
    UpTimeInfo(const std::string& uptimeFilePath = "/proc/uptime")
        : _uptime(0.0), _uptimeFilePath(uptimeFilePath) {}

    int update();
    double get() const { return _uptime; }
    std::string serialize() const;
};


// class Cpu
// {
// private:
//     /* data */
//     std::string cpuinfo;
// public:

// };

