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

#include "IHwMonitor.h"

#include <string>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <memory>

template <typename T>
std::string serialize(const T &obj)
{
    std::ostringstream oss;
    oss << obj;
    return oss.str();
}

class UpTimeInfo : public IHwMonitorTask
{
private:
    double _uptime;

protected:
    std::string _filePath;

public:
    UpTimeInfo() : _uptime{0.0}, _filePath{"/proc/uptime"} {};
    ~UpTimeInfo() = default;
    std::string getTaskName() const override { return "UpTimeInfo"; }
    std::string dumpToJSON() const override;

    int update() override;
    double get() const { return _uptime; }
    std::string serialize() const override { return ::serialize(*this); };

    friend std::ostream &operator<<(std::ostream &os, const UpTimeInfo &obj);
};

class LoadAvg : public IHwMonitorTask
{
private:
    double _load_1 = 0;
    double _load_5 = 0;
    double _load_15 = 0;

protected:
    std::string _filePath;

public:
    LoadAvg() : _filePath{"/proc/loadavg"} {};
    ~LoadAvg() override = default;
    std::string getTaskName() const override { return "LoadAvg"; }
    std::string dumpToJSON() const override;

    int update() override;
    std::tuple<double, double, double> get() const;
    std::string serialize() const override { return ::serialize(*this); };

    friend std::ostream &operator<<(std::ostream &os, const LoadAvg &obj);
};

class VersionInfo : public IHwMonitorTask
{
private:
    std::string _version;

protected:
    std::string _filePath;

public:
    VersionInfo() : _version(""), _filePath{"/proc/version"} {};
    ~VersionInfo() override = default;
    std::string getTaskName() const override { return "VersionInfo"; }
    std::string dumpToJSON() const override;

    int update() override;
    std::string get() const { return _version; }
    std::string serialize() const override { return ::serialize(*this); };

    friend std::ostream &operator<<(std::ostream &os, const VersionInfo &obj);
};

class MemInfo : public IHwMonitorTask
{
private:
    double _total;
    double _free;
    double _available;
    double _buffers;
    double _cached;
    double _swap_total;
    double _swap_free;
    double _swap_cached;

protected:
    std::string _filePath;

public:
    MemInfo() : _total{0.0}, _free{0.0}, _available{0.0}, _buffers{0.0}, _cached{0.0}, _swap_total{0.0}, _swap_free{0.0}, _swap_cached{0.0}, _filePath{"/proc/meminfo"} {};
    ~MemInfo() override = default;
    std::string getTaskName() const override { return "MemInfo"; }
    std::string dumpToJSON() const override;

    int update() override;
    std::tuple<double, double, double, double, double, double, double, double, double> get() const;
    double getMemTotal() const { return _total; };
    double getMemFree() const { return _free; };
    double getMemAvailable() const { return _available; };
    std::string serialize() const override { return ::serialize(*this); };

    friend std::ostream &operator<<(std::ostream &os, const MemInfo &obj);
};

class IpLinkStatistics : public IHwMonitorTask
{
private:
    int _rx_bytes = -1;
    int _rx_packets = -1;
    int _rx_errors = -1;
    int _rx_dropped = -1;
    int _tx_bytes = -1;
    int _tx_packets = -1;
    int _tx_errors = -1;
    int _tx_dropped = -1;
    std::string _interfaceName;

protected:
    int _readIntValueFromFile(const std::string &fileName);
    std::string _filePath;

public:
    IpLinkStatistics() : _interfaceName{"eth0"}, _filePath{"/sys/class/net/"} {};
    IpLinkStatistics(const std::string &interface) : _interfaceName{interface}, _filePath{"/sys/class/net/"} {};
    std::string getTaskName() const override { return "IpLinkStatistics/" + _interfaceName; }
    std::string dumpToJSON() const override;

    int update() override;
    void setIpLinkInterface(const std::string &interface) { _interfaceName = interface; };
    std::tuple<int, int, int, int, int, int, int, int> get() const;
    int getRxBytes() const { return _rx_bytes; };
    int getRxPackets() const { return _rx_packets; };
    int getRxErrors() const { return _rx_errors; };
    int getRxDropped() const { return _rx_dropped; };
    int getTxBytes() const { return _tx_bytes; };
    int getTxPackets() const { return _tx_packets; };
    int getTxErrors() const { return _tx_errors; };
    int getTxDropped() const { return _tx_dropped; };

    std::string serialize() const override { return ::serialize(*this); };

    friend std::ostream &operator<<(std::ostream &os, const IpLinkStatistics &obj);
};

class HwMonitor : public IHwMonitor
{
private:
    
    UpTimeInfo _upTimeInfo;
    LoadAvg _loadAvg;
    VersionInfo _versionInfo;
    MemInfo _memInfo;
    std::vector<std::shared_ptr<IHwMonitorTask>> _ipLinkStatistics;
    
    std::vector<std::string> _networkInterfaces;
    std::vector<std::shared_ptr<IHwMonitorTask>> _tasks;
public:
    HwMonitor();
    ~HwMonitor() override = default;
    void updateAll() override; 
    std::vector<std::shared_ptr<IHwMonitorTask>> getTasks() const override { return _tasks; }

protected:
    std::vector<std::string> listNetworkInterfaces();
};