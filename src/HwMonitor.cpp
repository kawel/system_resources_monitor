/**
 * @file: HwMonitor.cpp
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief:
 * -----
 * Copyright 2024 - KElectronics
 */

#include "HwMonitor.h"
#include "logger.h"

#include <iostream>
#include <fstream>
#include <sstream>

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
    std::ifstream uptimeFile(_uptimeFilePath);
    if (!uptimeFile.is_open())
    {
        std::cerr << "Failed to open " << _uptimeFilePath << std::endl;
        Logger::LogWarning("Failed to open " + _uptimeFilePath);
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

