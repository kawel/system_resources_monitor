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
#include <iostream>
#include <tuple>

class UpTimeInfo
{
private:
    double _uptime;

protected:
    std::string _filePath;

public:
    UpTimeInfo() : _uptime{0.0}, _filePath{"/proc/uptime"} {}

    int update();
    double get() const { return _uptime; }
    std::string serialize() const;

    // Friend declaration for the output stream operator
    friend std::ostream &operator<<(std::ostream &os, const UpTimeInfo &obj);
};

class LoadAvg
{
private:
    double _load_1 = 0;
    double _load_5 = 0;
    double _load_15 = 0;

protected:
    std::string _filePath;

public:
    LoadAvg() : _filePath{"/proc/loadavg"} {};
    int update();
    std::tuple<double, double, double> get() const;
    std::string serialize() const;

    friend std::ostream &operator<<(std::ostream &os, const LoadAvg &obj);
};

class VersionInfo
{
private:
    std::string _version;

protected:
    std::string _filePath;

public:
    VersionInfo() : _version(""), _filePath{"/proc/version"} {};
    int update();
    std::string get() const { return _version; }
    std::string serialize() const;

    friend std::ostream &operator<<(std::ostream &os, const VersionInfo &obj);
};

class MemInfo
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
    int update();
    std::tuple<double, double, double, double, double, double, double, double, double> get() const;
    double getMemTotal() const { return _total; };
    double getMemFree() const { return _free; };
    double getMemAvailable() const { return _available; };
    std::string serialize() const;

    friend std::ostream &operator<<(std::ostream &os, const MemInfo &obj);
};

class IpLinkStatistics
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
    int update();
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

    std::string serialize() const;

    friend std::ostream &operator<<(std::ostream &os, const IpLinkStatistics &obj);
};

