# System resources monitor

Monitor system resources and publish data to MQTT broker.

## About

This project is a simple system resources monitor that reads the CPU and memory usage and publishes the data to an MQTT broker. The data is published to the selected root topic in JSON format.

## Features

- Uptime - `/UpTimeInfo`
- Average CPU usage for 1, 5 and 15 minutes - `/LoadAvg`
- Memory usage - `/MemInfo`
- Version information - `/VersionInfo`
- Statistics for each ip link interface - `/IpLinkStatistics`
- Ability to work as a daemon
- Logs to syslog

## Requirements

`git submodule update --init --recursive`

```bash
sudo apt-get install pkg-config
# mosquitto
sudo apt-get install libmosquitto-dev
```

### RPI cross-compilation

```bash
# Install cross-compilation tools
sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
```

### Install service

Copy `sys_mon.service` to `/etc/systemd/system/`  
Reload systemd `sudo systemctl daemon-reload`  
Enable service `sudo systemctl enable sys_mon.service`  
Start service `sudo systemctl start sys_mon.service`  

### Logs

```bash
sudo journalctl -u sys_mon.service -f
```

Explanation of the options:

- -u sys_mon.service: Filters the logs to show only those related to the sys_mon service.
- -f: Follows the log output in real-time, similar to tail -f.

## Build

```bash
# Build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
# or
# cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

Clean build files

```bash
make clean
# remove build directory
rm -rf build
```

## How to use

### Arguments

- `-c` mqtt client name
- `-b` broker - MQTT broker address
- `-p` port - MQTT broker port
- `-k` keepAlive - MQTT keep alive
- `-u` user - MQTT user (not used in current implementation)
- `-w` password - MQTT password (not used in current implementation)
- `-r` topicRoot - MQTT topic root
- `-h` help - Show help

### Default values

- client_name: `sys_mon`
- broker: `localhost`
- port: `1883`
- keepAlive: `60`
- user: `None`
- password: `None`
- topicRoot: `sys_mon/data`

### Example

```bash
# Run the monitor with default values
./sys_mon

# Run the monitor with custom root topic
./sys_mon -r nad/sys_mon
```

## Release notes

### Version 1.1.2

- Add client name to command line arguments

### Version 1.1.1

- fix json key for load average
- extend output for memory usage

### Version 1.1.0

- Refactor build system to be used in cross-compilation
- Minor code improvements after code review
- Add uninstall target to Makefile
- Add scripts easy the build process
- Add .devcontainer for VSCode

### Version 1.0.0

- Initial version with a basic set of features
