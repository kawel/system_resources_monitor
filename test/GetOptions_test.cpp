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

TEST(GetOptions, GetOptions)
{
    MqttCfg mqtt_cfg = mqtt_cfg_vanilla;

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

TEST(GetOptions, GetOptionsInvalid)
{
    MqttCfg mqtt_cfg("client_name", "host_address", 1111, 22, "user_name", "user_password", "root_topic");
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

    // Check if the mqtt_cfg is not changed
    EXPECT_EQ(mqtt_cfg._clientName, mqtt_cfg_vanilla._clientName);
    EXPECT_EQ(mqtt_cfg._host, mqtt_cfg_vanilla._host);
    EXPECT_EQ(mqtt_cfg._port, mqtt_cfg_vanilla._port);
    EXPECT_EQ(mqtt_cfg._keepAlive, mqtt_cfg_vanilla._keepAlive);
    EXPECT_EQ(mqtt_cfg._username, mqtt_cfg_vanilla._username);
    EXPECT_EQ(mqtt_cfg._password, mqtt_cfg_vanilla._password);
}