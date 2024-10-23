
// #include <gtest/gtest.h>


// TEST(MyTestSuite, MyTestCase) {
//     // Your test code here
//     ASSERT_TRUE(true);
// }

// // Demonstrate some basic assertions.
// TEST(HelloTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
// }

#include "HwMonitor.h"

#include <gtest/gtest.h>
#include <fstream>
#include <string>


// Helper function to mock /proc/uptime content
void mockUptimeFile(const std::string& content) {
    std::ofstream uptimeFile("/tmp/mock_uptime");
    uptimeFile << content;
    uptimeFile.close();
}

TEST(UpTimeInfoTest, UpdateSuccess) {
    mockUptimeFile("12345.67 54321.00");
    UpTimeInfo uptimeInfo;
    EXPECT_EQ(uptimeInfo.update(), 0);
    EXPECT_DOUBLE_EQ(uptimeInfo.get(), 12345.67);
}

// TEST(UpTimeInfoTest, UpdateFail) {
//     mockUptimeFile(""); // Empty content to simulate failure
//     UpTimeInfo uptimeInfo;
//     EXPECT_EQ(uptimeInfo.update(), -1);
//     EXPECT_DOUBLE_EQ(uptimeInfo.get(), -1);
// }