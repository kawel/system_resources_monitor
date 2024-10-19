#include <iostream>

#include "logger.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello, World!" << std::endl;
    Logger::Initialize(LOG_NOTICE, "symon");
    Logger::Log(NOTICE, "System monitor is starting...");

    Logger::Deinit();
    return 0;
}
