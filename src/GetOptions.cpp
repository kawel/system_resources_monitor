/**
 * @file: GetOptions.cpp
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief: Get options from command line
 * -----
 * Copyright 2024 - KElectronics
 */
#include "GetOptions.h"

#include <cstring>
#include <getopt.h>

#include "logger.h"

int getOptions(MqttCfg &mqtt_cfg, int argc, char *argv[])
{
    MqttCfg mqtt_cfg_tmp = mqtt_cfg;
    int opt;

    while ((opt = getopt(argc, argv, "b:p:k:u:w:r:h")) != -1)
    {
        switch (opt)
        {
        case 'b':
            mqtt_cfg_tmp._host = optarg;
            break;
        case 'p':
            mqtt_cfg_tmp._port = std::stoi(optarg);
            break;
        case 'k':
            mqtt_cfg_tmp._keepAlive = std::stoi(optarg);
            break;
        case 'u':
            mqtt_cfg_tmp._username = optarg;
            break;
        case 'w':
            mqtt_cfg_tmp._password = optarg;
            break;
        case 'r':
            mqtt_cfg_tmp.setRootTopic(optarg);
            break;
        case 'h':
            std::cout << "Usage: " << argv[0] << " [-b broker] [-p port] [-k keepAlive] [-u user] [-w password] [-r topicRoot] [-h help]\n";
            return 1;
        case '?':
        default:
            std::cout << "Usage: " << argv[0] << " [-b broker] [-p port] [-k keepAlive] [-u user] [-w password] [-r topicRoot] [-h help]\n";
            return -1;
        }
    }

    mqtt_cfg = mqtt_cfg_tmp;
    return 0;
}