/**
 * @file: GetOptions_test.cpp
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief: Get options from command line tests
 * -----
 * Copyright 2024 - KElectronics
 */

#include "GetOptions.h"
#include <fstream>
#include <string>
#include <tuple>
#include <gtest/gtest.h>

const MqttCfg mqtt_cfg_vanilla("client_name", "host_address", 1111, 22, "user_name", "user_password", "root_topic");

class GetOptionsTest : public ::testing::Test {
protected:
    MqttCfg mqtt_cfg;

    void SetUp() override {
        mqtt_cfg = mqtt_cfg_vanilla;
    }

    void TearDown() override {
        // Clean up resources if needed
    }

    int checkIfMqttCfgIsNotChanged(const MqttCfg &mqtt_cfg, const MqttCfg &mqtt_cfg_vanilla)
    {
        // Check if the mqtt_cfg is not changed
        if (mqtt_cfg._clientName != mqtt_cfg_vanilla._clientName ||
            mqtt_cfg._host != mqtt_cfg_vanilla._host ||
            mqtt_cfg._port != mqtt_cfg_vanilla._port ||
            mqtt_cfg._keepAlive != mqtt_cfg_vanilla._keepAlive ||
            mqtt_cfg._username != mqtt_cfg_vanilla._username ||
            mqtt_cfg._password != mqtt_cfg_vanilla._password)
        {
            return -1;
        }
        return 0;
    }
};

TEST_F(GetOptionsTest, GetOptions)
{
    int argc = 7;
    char *argv[] = {
        (char *)"test",
        (char *)"-b",
        (char *)"localhost",
        (char *)"-p",
        (char *)"1883",
        (char *)"-k",
        (char *)"60"};

    int ret_val = getOptions(mqtt_cfg, argc, argv);
    EXPECT_EQ(ret_val, 0);

    EXPECT_EQ(mqtt_cfg._clientName, "client_name");
    EXPECT_EQ(mqtt_cfg._host, "localhost");
    EXPECT_EQ(mqtt_cfg._port, 1883);
    EXPECT_EQ(mqtt_cfg._keepAlive, 60);
    EXPECT_EQ(mqtt_cfg._username, "user_name");
    EXPECT_EQ(mqtt_cfg._password, "user_password");
}

TEST_F(GetOptionsTest, GetOptionsInvalid)
{
    int argc = 8;
    char *argv[] = {
        (char *)"test",
        (char *)"-b",
        (char *)"localhost",
        (char *)"-p",
        (char *)"1883",
        (char *)"-k",
        (char *)"60",
        (char *)"-x"};

    EXPECT_EQ(getOptions(mqtt_cfg, argc, argv), -1);

    EXPECT_EQ(checkIfMqttCfgIsNotChanged(mqtt_cfg, mqtt_cfg_vanilla), 0);
}

TEST_F(GetOptionsTest, MANUAL_GetOptionsHelp)
{
    int argc = 2;
    char *argv[] = {
        (char *)"test",
        (char *)"-h"};

    // Capture stdout
    testing::internal::CaptureStdout();

    int ret_val = getOptions(mqtt_cfg, argc, argv);

    // Stop capturing stdout
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(ret_val, 1);

    EXPECT_EQ(checkIfMqttCfgIsNotChanged(mqtt_cfg, mqtt_cfg_vanilla), 0);

    // Check if the help message is printed
    EXPECT_NE(output.find("Usage: test [-b broker] [-p port] [-k keepAlive] [-u user] [-w password] [-r topicRoot] [-h help]"), std::string::npos);
}

TEST_F(GetOptionsTest, MANUAL_GetOptionsNoArguments)
{
    int argc = 1;
    char *argv[] = {
        (char *)"test"};

    EXPECT_EQ(getOptions(mqtt_cfg, argc, argv), 0);

    EXPECT_EQ(checkIfMqttCfgIsNotChanged(mqtt_cfg, mqtt_cfg_vanilla), 0);


    int argc2 = 0;
    char *argv2[] = {};

    EXPECT_EQ(getOptions(mqtt_cfg, argc2, argv2), 0);

    EXPECT_EQ(checkIfMqttCfgIsNotChanged(mqtt_cfg, mqtt_cfg_vanilla), 0);

}