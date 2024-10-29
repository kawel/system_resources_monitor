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
#include <dirent.h> // Include POSIX directory handling

#include "nlohmann/json.hpp"
// #include "/mnt/e/projects/posejdon_system/apps/system_resorces_monitor/ext/json/single_include/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

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

std::string UpTimeInfo::dumpToJSON() const
{
    json j;
    j["uptime"] = {
        {"value", _uptime},
        {"unit", "s"}
    };
    return j.dump();
}

std::ostream &operator<<(std::ostream &os, const UpTimeInfo &obj)
{
    os << std::fixed << std::setprecision(2) << obj._uptime << "[s]";

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

std::tuple<double, double, double> LoadAvg::get() const
{
    return std::make_tuple(_load_1, _load_5, _load_15);
}

std::string LoadAvg::dumpToJSON() const
{
    json j;
    j["load"] = {
        {"1min", _load_1},
        {"5min", _load_5},
        {"15min", _load_15},
        {"unit", "%"}
    };
    return j.dump();
}

std::ostream &operator<<(std::ostream &os, const LoadAvg &obj)
{
    os << std::fixed << std::setprecision(2) << obj._load_1 << "[%], " << obj._load_5 << "[%], " << obj._load_15 << "[%]";

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

std::string VersionInfo::dumpToJSON() const
{
    json j;
    j["version"] = _version;
    return j.dump();
}

std::ostream &operator<<(std::ostream &os, const VersionInfo &obj)
{
    os << obj._version;

    return os;
}

int MemInfo::update()
{
    std::ifstream memInfoFile(_filePath);
    if (!memInfoFile.is_open())
    {
        Logger::LogWarning("Failed to open " + _filePath);
        _total = _free = _available = -1;
        _buffers = _cached = -1;
        _swap_total = _swap_free = _swap_cached = -1;
        return -1;
    }

    std::string line;
    std::getline(memInfoFile, line);
    if (line.empty())
    {
        _total = _free = _available = -1;
        _buffers = _cached = -1;
        _swap_total = _swap_free = _swap_cached = -1;

        memInfoFile.close();
        return -1;
    }

    do
    {

        std::istringstream iss(line);
        std::string key;
        iss >> key;
        if (key == "MemTotal:")
        {
            iss >> _total;
        }
        else if (key == "MemFree:")
        {
            iss >> _free;
        }
        else if (key == "MemAvailable:")
        {
            iss >> _available;
            break;
        }
    } while (std::getline(memInfoFile, line));

    memInfoFile.close();

    return 0;
}

std::string MemInfo::dumpToJSON() const
{
    json j;
    j["MemInfo"] = {
        {"MemTotal", _total},
        {"MemFree", _free},
        {"MemAvailable", _available},
        {"Buffers", _buffers},
        {"Cached", _cached},
        {"SwapTotal", _swap_total},
        {"SwapFree", _swap_free},
        {"SwapCached", _swap_cached},
        {"unit", "kB"}
    };

    return j.dump();
}

std::ostream &operator<<(std::ostream &os, const MemInfo &obj)
{
    os << std::fixed << std::setprecision(2) << "MemTotal: " << obj._total << " kB, MemFree: " << obj._free << " kB, MemAvailable: " << obj._available << " kB";

    return os;
}

int IpLinkStatistics::_readIntValueFromFile(const std::string &fileName)
{
    std::string filePath = _filePath + _interfaceName + "/statistics/" + fileName;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        Logger::LogWarning("Failed to open " + filePath);
        return -1;
    }

    int value;
    file >> value;
    file.close();

    return value;
}

std::tuple<int, int, int, int, int, int, int, int> IpLinkStatistics::get() const
{
    return std::make_tuple(_rx_bytes, _rx_packets, _rx_errors, _rx_dropped, _tx_bytes, _tx_packets, _tx_errors, _tx_dropped);
}

int IpLinkStatistics::update()
{
    // check if the interface directory exists
    std::string interfaceDir = _filePath + _interfaceName;
    std::ifstream dir(interfaceDir);
    if (!dir.is_open())
    {
        Logger::LogWarning("Failed to open " + interfaceDir);
        return -1;
    }

    _rx_bytes = _readIntValueFromFile("rx_bytes");
    _rx_packets = _readIntValueFromFile("rx_packets");
    _rx_errors = _readIntValueFromFile("rx_errors");
    _rx_dropped = _readIntValueFromFile("rx_dropped");
    _tx_bytes = _readIntValueFromFile("tx_bytes");
    _tx_packets = _readIntValueFromFile("tx_packets");
    _tx_errors = _readIntValueFromFile("tx_errors");
    _tx_dropped = _readIntValueFromFile("tx_dropped");

    return 0;
}

std::string IpLinkStatistics::dumpToJSON() const
{
    json j;
    j[_interfaceName] ={
        {"rx_bytes", _rx_bytes},
        {"rx_packets", _rx_packets},
        {"rx_errors", _rx_errors},
        {"rx_dropped", _rx_dropped},
        {"tx_bytes", _tx_bytes},
        {"tx_packets", _tx_packets},
        {"tx_errors", _tx_errors},
        {"tx_dropped", _tx_dropped},
        {"unit", "bytes"}
    };
    return j.dump();
}

std::ostream &operator<<(std::ostream &os, const IpLinkStatistics &obj)
{
    os << "RX: " << obj._rx_bytes << " bytes, " << obj._rx_packets << " packets, " << obj._rx_errors << " errors, " << obj._rx_dropped << " dropped" << std::endl;
    os << "TX: " << obj._tx_bytes << " bytes, " << obj._tx_packets << " packets, " << obj._tx_errors << " errors, " << obj._tx_dropped << " dropped" << std::endl;

    return os;
}

HwMonitor::HwMonitor() : _upTimeInfo{}, _loadAvg{},
                         _versionInfo{}, _memInfo{},
                         _networkInterfaces{}
{
    _networkInterfaces = listNetworkInterfaces();
    for (const auto &interface : _networkInterfaces)
    {   
        if (interface == "lo")
        {
            continue;
        }

        _ipLinkStatistics.push_back(std::make_shared<IpLinkStatistics>(IpLinkStatistics(interface)));
    }

}

std::vector<std::string> HwMonitor::listNetworkInterfaces()
{
    // function listing folders in /sys/class/net
    std::vector<std::string> interfaces;
    std::string path = "/sys/class/net";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_LNK)
            {
                std::string name = ent->d_name;
                if (name != "." && name != "..")
                {
                    interfaces.push_back(name);
                }
            }
        }
        closedir(dir);
    }
    else
    {
        Logger::LogError("Failed to open " + path);
    }

    return interfaces;
}