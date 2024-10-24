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
#include <iomanip>


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
    std::ifstream uptimeFile(_filePath);
    if (!uptimeFile.is_open())
    {
        Logger::LogWarning("Failed to open " + _filePath);
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

// Define the output stream operator for UpTimeInfo
std::ostream &operator<<(std::ostream &os, const UpTimeInfo &obj)
{
    os << std::fixed << std::setprecision(2) << obj._uptime <<"[s]";

    return os;
}

int LoadAvg::update()
{
    std::ifstream loadAvgFile(_filePath);
    if (!loadAvgFile.is_open())
    {
        Logger::LogWarning("Failed to open " + _filePath);
        _load_1 = _load_5 = _load_15 = -1;
        return -1;
    }

    std::string line;
    std::getline(loadAvgFile, line);
    loadAvgFile.close();

    std::istringstream iss(line);
    iss >> _load_1 >> _load_5 >> _load_15;

    return 0;
}

std::tuple<double, double, double> LoadAvg::get() const {
    return std::make_tuple(_load_1, _load_5, _load_15);
}

std::ostream &operator<<(std::ostream &os, const LoadAvg &obj)
{
    os << std::fixed << std::setprecision(2) << obj._load_1 << "[%], " << obj._load_5 << "[%], " << obj._load_15 <<"[%]";

    return os;
}

int VersionInfo::update()
{
    std::ifstream versionFile(_filePath);
    if (!versionFile.is_open())
    {
        Logger::LogWarning("Failed to open " + _filePath);
        _version = "N/A";
        return -1;
    }

    std::string line;
    std::getline(versionFile, line);
    // Check if line is empty
    if (line.empty())
    {
        Logger::LogWarning("Failed to read version info from " + _filePath);
        _version = "N/A";
        return -1;
    }
    versionFile.close();

    _version = line;

    return 0;
}

std::ostream &operator<<(std::ostream &os, const VersionInfo &obj)
{
    os << obj._version;

    return os;
}