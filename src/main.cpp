#include <iostream>

#include "logger.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;
    Logger::Initialize();
    Logger::Log("Starting Fuba Smart Antenna Manager... ");
    return 0;
}
