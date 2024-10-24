#include "HwMonitor.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <tuple>

// Helper function to mock /proc/uptime content
void mockUpFile(const std::string &filePath, const std::string &content)
{
    std::ofstream mockFile(filePath);
    mockFile << content;
    mockFile.close();
}

class TestUpTimeInfo : public UpTimeInfo
{
public:
    void setFilePath(const std::string &filePath) { _filePath = filePath;};
};

TEST(HwMonitorUpTimeInfo, UpdateSuccess)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUpFile(mockFilePath, "12345.67 8910.11");

    TestUpTimeInfo uptimeInfo;
    uptimeInfo.setFilePath(mockFilePath);
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_DOUBLE_EQ(uptimeInfo.get(), 12345.67);
}

TEST(HwMonitorUpTimeInfo, UpdateFailed)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUpFile(mockFilePath, "-42"); // Empty content to simulate failure

    TestUpTimeInfo uptimeInfo;
    uptimeInfo.setFilePath(mockFilePath);
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_DOUBLE_EQ(uptimeInfo.get(), -42);
}

TEST(HwMonitorUpTimeInfo, Ostream)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUpFile(mockFilePath, "12345.67 8910.11");

    TestUpTimeInfo uptimeInfo;
    uptimeInfo.setFilePath(mockFilePath);
    uptimeInfo.update();

    std::ostringstream oss;
    oss << uptimeInfo;
    EXPECT_EQ(oss.str(), "12345.67[s]");
}

TEST(HwMonitorUpTimeInfo, FunctionalTest)
{
    UpTimeInfo uptimeInfo;
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_GE(uptimeInfo.get(), 0);

    std::cout << "UpTime: " << uptimeInfo.get() << std::endl;
}


class TestLoadAvg : public LoadAvg
{
public:
    void setFilePath(const std::string &filePath) { _filePath = filePath;};
};

TEST(HwMonitorLoadAvg, UpdateSuccess)
{
    TestLoadAvg loadAvg{};

    const std::string mockFilePath = "/tmp/mock_loadavg";
    loadAvg.setFilePath(mockFilePath);
    mockUpFile(mockFilePath, "1.23 4.56 7.89 10/100 12345");

    EXPECT_EQ(loadAvg.update(), 0);

    double load1, load5, load15;
    std::tie(load1, load5, load15) = loadAvg.get();
    EXPECT_DOUBLE_EQ(load1, 1.23);
    EXPECT_DOUBLE_EQ(load5, 4.56);
    EXPECT_DOUBLE_EQ(load15, 7.89);
}

TEST(HwMonitorLoadAvg, UpdateFailed)
{
    const std::string mockFilePath = "/tmp/mock_loadavg";
    mockUpFile(mockFilePath, " "); // Empty content to simulate failure

    TestLoadAvg loadAvg{};
    loadAvg.setFilePath(mockFilePath);
    EXPECT_EQ(loadAvg.update(), 0);

    double load1, load5, load15;
    std::tie(load1, load5, load15) = loadAvg.get();
    EXPECT_DOUBLE_EQ(load1, 0);
    EXPECT_DOUBLE_EQ(load5, 0);
    EXPECT_DOUBLE_EQ(load15, 0);
}

TEST(HwMonitorLoadAvg, Ostream)
{
    const std::string mockFilePath = "/tmp/mock_loadavg";
    mockUpFile(mockFilePath, "1.23 4.56 7.89 10/100 12345");

    TestLoadAvg loadAvg{};
    loadAvg.setFilePath(mockFilePath);
    loadAvg.update();

    std::ostringstream oss;
    oss << loadAvg;
    EXPECT_EQ(oss.str(), "1.23[%], 4.56[%], 7.89[%]");
}

TEST(HwMonitorLoadAvg, FunctionalTest)
{
    TestLoadAvg loadAvg;
    EXPECT_EQ(loadAvg.update(), 0);

    double load1, load5, load15;
    std::tie(load1, load5, load15) = loadAvg.get();
    EXPECT_GE(load1, 0);
    EXPECT_GE(load5, 0);
    EXPECT_GE(load15, 0);

    std::cout << "Load Average: " << load1 << ", " << load5 << ", " << load15 << std::endl;
}

class TestVersionInfo : public VersionInfo
{
public:
    using VersionInfo::update; // Expose the protected method as public
    void setFilePath(const std::string &filePath) { _filePath = filePath;};// Expose the protected member as public
};

TEST(HwMonitorVersionInfo, UpdateSuccess)
{
    const std::string mockFilePath = "/tmp/mock_version";
    mockUpFile(mockFilePath, "Linux version 5.4.0-42-generic (build@lgw01-amd64-039) (gcc version 9.3.0 (Ubuntu 9.3.0-10ubuntu2)) #46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020");

    TestVersionInfo versionInfo{};
    versionInfo.setFilePath(mockFilePath);
    versionInfo.update();
    EXPECT_EQ(versionInfo.get(), "Linux version 5.4.0-42-generic (build@lgw01-amd64-039) (gcc version 9.3.0 (Ubuntu 9.3.0-10ubuntu2)) #46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020");
}

TEST(HwMonitorVersionInfo, UpdateFailed_EmptyContent)
{
    const std::string mockFilePath = "/tmp/mock_version";
    mockUpFile(mockFilePath, ""); // Empty content to simulate failure

    TestVersionInfo versionInfo{};
    versionInfo.setFilePath(mockFilePath);
    versionInfo.update();
    EXPECT_EQ(versionInfo.get(), "N/A");
}

TEST(HwMonitorVersionInfo, UpdateFailed_NoFile)
{
    const std::string mockFilePath = "/tmp/mock_version";

    TestVersionInfo versionInfo{};
    versionInfo.setFilePath(mockFilePath);
    versionInfo.update();
    EXPECT_EQ(versionInfo.get(), "N/A");
}

TEST(HwMonitorVersionInfo, Ostream)
{
    const std::string mockFilePath = "/tmp/mock_version";
    mockUpFile(mockFilePath, "Linux version 5.4.0-42-generic (build@lgw01-amd64-039) (gcc version 9.3.0 (Ubuntu 9.3.0-10ubuntu2)) #46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020");

    TestVersionInfo versionInfo{};
    versionInfo.setFilePath(mockFilePath);
    versionInfo.update();

    std::ostringstream oss;
    oss << versionInfo;
    EXPECT_EQ(oss.str(), "Linux version 5.4.0-42-generic (build@lgw01-amd64-039) (gcc version 9.3.0 (Ubuntu 9.3.0-10ubuntu2)) #46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020");
}

TEST(HwMonitorVersionInfo, FunctionalTest)
{
    VersionInfo versionInfo;
    EXPECT_EQ(versionInfo.update(), 0);
    EXPECT_NE(versionInfo.get(), "N/A");

    std::cout << "Version: " << versionInfo.get() << std::endl;
}

class TestMemInfo : public MemInfo
{
public:
    void setFilePath(const std::string &filePath) { _filePath = filePath;};// Expose the protected member as public
};

TEST(HwMonitorMemInfo, UpdateSuccess)
{
    const std::string mockFilePath = "/tmp/mock_meminfo";
    mockUpFile(mockFilePath, "MemTotal: 16384 kB\nMemFree: 1234 kB\nMemAvailable: 5678 kB");

    TestMemInfo memInfo{};
    memInfo.setFilePath(mockFilePath);
    EXPECT_EQ(memInfo.update(), 0);
    EXPECT_GT(memInfo.getMemTotal(), 0);
    EXPECT_GT(memInfo.getMemFree(), 0);
    EXPECT_GT(memInfo.getMemAvailable(), 0);
}

TEST(HwMonitorMemInfo, UpdateFailed)
{
    const std::string mockFilePath = "/tmp/mock_meminfo";
    mockUpFile(mockFilePath, ""); // Empty content to simulate failure

    TestMemInfo memInfo{};
    memInfo.setFilePath(mockFilePath);
    EXPECT_EQ(memInfo.update(), -1);
    EXPECT_EQ(memInfo.getMemTotal(), -1);
    EXPECT_EQ(memInfo.getMemFree(), -1);
    EXPECT_EQ(memInfo.getMemAvailable(), -1);

    memInfo.setFilePath("/tmp/mxx");
    EXPECT_EQ(memInfo.update(), -1);    
    EXPECT_EQ(memInfo.getMemTotal(), -1);
    EXPECT_EQ(memInfo.getMemFree(), -1);
    EXPECT_EQ(memInfo.getMemAvailable(), -1);
}

TEST(HwMonitorMemInfo, FunctionalTest)
{
    MemInfo memInfo;
    EXPECT_EQ(memInfo.update(), 0);
    EXPECT_GT(memInfo.getMemTotal(), 0);
    EXPECT_GT(memInfo.getMemFree(), 0);
    EXPECT_GT(memInfo.getMemAvailable(), 0);

    std::cout << "Total memory: " << memInfo.getMemTotal() << std::endl;
    std::cout << memInfo;
}

class TestIpLinkStatistics : public IpLinkStatistics
{
    public:
        void setFilePath(const std::string &filePath) { _filePath = filePath;};
        TestIpLinkStatistics(const std::string &interface) : IpLinkStatistics{interface} {};
        using IpLinkStatistics::_readIntValueFromFile;
};

void createFileWithDirectories(const std::string& filePath, const std::string& content) {
    // Extract the directory path from the file path
    std::string dirPath = filePath.substr(0, filePath.find_last_of('/'));

    // Create the directory structure using mkdir -p
    std::string command = "mkdir -p " + dirPath;
    if (system(command.c_str()) != 0) {
        std::cerr << "Failed to create directories: " << dirPath << std::endl;
        return;
    }

    // Create and write to the file
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        std::cerr << "Failed to create file: " << filePath << std::endl;
    }
}

TEST(HwMonitorIpLinkStatistics, _readIntValueFromFile)
{
    const std::string mockFilePath = "/tmp/eth0/statistics/mock_ip_link";
    const std::string content = "12345";

    createFileWithDirectories(mockFilePath, content);
    // mockUpFile(mockFilePath, "12345");

    TestIpLinkStatistics ipLinkStatistics{"eth0"};
    ipLinkStatistics.setFilePath("/tmp/");
    EXPECT_EQ(ipLinkStatistics._readIntValueFromFile("mock_ip_link"), 12345);

    EXPECT_EQ(ipLinkStatistics._readIntValueFromFile("mock_ip_linkX"), -1);

    mockUpFile(mockFilePath, "d|_|pa");
    EXPECT_EQ(ipLinkStatistics._readIntValueFromFile("mock_ip_link"), 0);

}


TEST(HwMonitorIpLinkStatistics, UpdateSuccess)
{
    const std::string mockFilePath = "/tmp/eth0/statistics/rx_bytes";
    const std::string content = "12345";

    createFileWithDirectories(mockFilePath, content);

    TestIpLinkStatistics ipLinkStatistics{"eth0"};
    ipLinkStatistics.setFilePath("/tmp/");
    EXPECT_EQ(ipLinkStatistics.update(), 0);

    EXPECT_EQ(ipLinkStatistics.getRxBytes(), 12345);
    EXPECT_EQ(ipLinkStatistics.getTxBytes(), -1);
}

TEST(HwMonitorIpLinkStatistics, FunstionalTest)
{
    IpLinkStatistics ipLinkStatistics;
    EXPECT_EQ(ipLinkStatistics.update(), 0);

    int rxBytes, rxPackets, rxErrors, rxDropped, txBytes, txPackets, txErrors, txDropped;
    std::tie(rxBytes, rxPackets, rxErrors, rxDropped, txBytes, txPackets, txErrors, txDropped) = ipLinkStatistics.get();
    EXPECT_GE(rxBytes, 0);
    EXPECT_GE(rxPackets, 0);
    EXPECT_GE(rxErrors, 0);
    EXPECT_GE(rxDropped, 0);
    EXPECT_GE(txBytes, 0);
    EXPECT_GE(txPackets, 0);
    EXPECT_GE(txErrors, 0);
    EXPECT_GE(txDropped, 0);

    std::cout << "RxBytes: " << rxBytes << std::endl;
    std::cout << "TxBytes: " << txBytes << std::endl;
}

TEST(HwMonitorIpLinkStatistics, OStream)
{
    IpLinkStatistics ipLinkStatistics("eth0");
    ipLinkStatistics.update();
    std::cout << ipLinkStatistics;

    IpLinkStatistics ipLinkStatistics2("lo");
    ipLinkStatistics2.update();
    std::cout << ipLinkStatistics2;

    IpLinkStatistics ipLinkStatistics3("bad_ifc");
    ipLinkStatistics3.update();
    std::cout << ipLinkStatistics3;
}