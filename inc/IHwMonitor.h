/**
 * @file: IHwMonitor.h
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief: Interface for hardware monitoring tasks
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
#include <memory>
#include <vector>

class IHwMonitorTask
{
public:
    virtual int update() = 0;
    virtual std::string serialize() const = 0;
    virtual ~IHwMonitorTask() = default;
    std::string run()
    {
        update();
        return serialize();
    };
};

class IHwMonitor
{
public:
    virtual void updateAll() = 0;
    virtual ~IHwMonitor() = default;
    virtual std::vector<std::shared_ptr<IHwMonitorTask>>  getTasks() const = 0;

};