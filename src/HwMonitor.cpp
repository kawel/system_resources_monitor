/**
 * @file: HwMonitor.cpp
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief:
 * -----
 * Copyright 2024 - KElectronics
 */

#include "HwMonitor.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "logger.h"

// HwMonitor::HwMonitor()
// {
// }

// HwMonitor::~HwMonitor()
// {
// }

// int HwMonitor::getUpTime()
// {
//     return 0;
// }

int UpTimeInfo::update()
{
    std::ifstream uptimeFile("/proc/uptime");
    if (!uptimeFile.is_open())
    {
        std::cerr << "Failed to open /proc/uptime" << std::endl;
        Logger::LogWarning("Failed to open /proc/uptime");
        _uptime = -1;
        return -1;
    }

    std::string line;
    std::getline(uptimeFile, line);
    uptimeFile.close();

    std::istringstream iss(line);
    double uptime;
    iss >> uptime;
    _uptime = uptime;

    return 0;
}

