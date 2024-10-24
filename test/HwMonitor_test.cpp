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

TEST(HwMonitorUpTimeInfo, UpdateSuccess)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUpFile(mockFilePath, "12345.67 8910.11");

    UpTimeInfo uptimeInfo(mockFilePath);
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_DOUBLE_EQ(uptimeInfo.get(), 12345.67);
}

TEST(HwMonitorUpTimeInfo, UpdateFailed)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUpFile(mockFilePath, "-42"); // Empty content to simulate failure

    UpTimeInfo uptimeInfo(mockFilePath);
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_DOUBLE_EQ(uptimeInfo.get(), -42);
}

TEST(HwMonitorUpTimeInfo, Ostream)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUpFile(mockFilePath, "12345.67 8910.11");

    UpTimeInfo uptimeInfo(mockFilePath);
    uptimeInfo.update();

    std::ostringstream oss;
    oss << uptimeInfo;
    EXPECT_EQ(oss.str(), "12345.67[s]");
}

TEST(HwMonitorUpTimeInfo, FunftionalTest)
{
    UpTimeInfo uptimeInfo;
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_GE(uptimeInfo.get(), 0);

    std::cout << "UpTime: " << uptimeInfo.get() << std::endl;
}


class TestLoadAvg : public LoadAvg
{
public:
    using LoadAvg::setFilePath; // Expose the protected method as public
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
    mockUpFile(mockFilePath, "Linux version 5.4.0-42-generic (buildd@lgw01-amd64-039) (gcc version 9.3.0 (Ubuntu 9.3.0-10ubuntu2)) #46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020");

    TestVersionInfo versionInfo{};
    versionInfo.setFilePath(mockFilePath);
    versionInfo.update();
    EXPECT_EQ(versionInfo.get(), "Linux version 5.4.0-42-generic (buildd@lgw01-amd64-039) (gcc version 9.3.0 (Ubuntu 9.3.0-10ubuntu2)) #46-Ubuntu SMP Fri Jul 10 00:24:02 UTC 2020");
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
