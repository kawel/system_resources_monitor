cmake_minimum_required(VERSION 3.10)

# Define the project name
project(sys_mon)

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable verbose makefile
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Define an option to include or exclude unit tests
option(BUILD_UNIT_TESTS "Build unit tests" OFF)

# Set the include directories
set(INCLUDES inc)

# Set the source files
set(SOURCES 
    src/GetOptions.cpp
    src/HwMonitor.cpp
    src/logger.cpp
    src/main.cpp
    src/MqttClient.cpp
    src/Scheduler.cpp
    src/SysMonitor.cpp
)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Add the external json directory
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/json ${CMAKE_BINARY_DIR}/ext/json)

# Include the specified include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# Set compiler flags for Debug and Release builds
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -Wall -fmessage-length=0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -fmessage-length=0")

set(LIBS nlohmann_json::nlohmann_json mosquitto pthread)

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Add compile definitions for Release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
    USE_SYSLOG
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
    USE_SYSLOG 
    USE_STDOUT
    )
endif()

# Execute the command and capture the output
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine OUTPUT_VARIABLE GCC_MACHINE)

# Strip the trailing newline
string(STRIP "${GCC_MACHINE}" GCC_MACHINE)

# Print the build type
message("====================================================")
message(STATUS "[INFO] Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "[INFO] Compile unit tests: ${BUILD_UNIT_TESTS}")
message(STATUS "[INFO] Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "[INFO] Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "[INFO] Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "[INFO] Compiler machine: ${GCC_MACHINE}")
# Print all used target definitions
get_target_property(TARGET_COMPILE_DEFINITIONS ${PROJECT_NAME} COMPILE_DEFINITIONS)
message(STATUS "[INFO] Target compile definitions: ${TARGET_COMPILE_DEFINITIONS}")
message("====================================================")

if(BUILD_UNIT_TESTS MATCHES ON) 
    enable_testing()
    add_subdirectory(test)
endif() 

# Ensure the destination directory exists
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory /usrdata/bin)")

# Install procedure
install(TARGETS sys_mon DESTINATION /usrdata/bin/)
install(FILES ${CMAKE_SOURCE_DIR}/etc/systemd/sys_mon.service DESTINATION /etc/systemd/system)

# Include the uninstall procedure
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY
)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)