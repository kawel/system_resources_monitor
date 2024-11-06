# System resources monitor

Monitor system resources and publish data to MQTT broker.

Version 1.0.0

## Requirements

`git submodule update --init --recursive`

```bash
sudo apt-get install pkg-config
# mosquitto
sudo apt-get install libmosquitto-dev
```

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

## How to use

### Arguments

- `-b` broker - MQTT broker address
- `-p` port - MQTT broker port
- `-k` keepAlive - MQTT keep alive
- `-u` user - MQTT user (not used in current implementation)
- `-w` password - MQTT password (not used in current implementation)
- `-r` topicRoot - MQTT topic root
- `-h` help - Show help

### Default values

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

### Version 1.0.0

- Initial version with a basic set of features
