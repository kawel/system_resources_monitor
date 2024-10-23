#include "HwMonitor.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>

// Helper function to mock /proc/uptime content
void mockUptimeFile(const std::string &filePath, const std::string &content)
{
    std::ofstream mockFile(filePath);
    mockFile << content;
    mockFile.close();
}

TEST(HwMonitorUpTimeInfo, UpdateSuccess)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUptimeFile(mockFilePath, "12345.67 8910.11");

    UpTimeInfo uptimeInfo(mockFilePath);
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_DOUBLE_EQ(uptimeInfo.get(), 12345.67);
}

TEST(HwMonitorUpTimeInfo, UpdateFailed)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUptimeFile(mockFilePath, "-42"); // Empty content to simulate failure

    UpTimeInfo uptimeInfo(mockFilePath);
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_DOUBLE_EQ(uptimeInfo.get(), -42);
}

TEST(HwMonitorUpTimeInfo, Ostream)
{
    const std::string mockFilePath = "/tmp/mock_uptime";
    mockUptimeFile(mockFilePath, "12345.67 8910.11");

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

