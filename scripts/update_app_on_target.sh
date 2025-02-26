#!/bin/bash

# Update application on target over the network using sshpass and scp

# Variables
USER=root
HOST=192.168.225.1
PASSWORD=oelinux123
APP_NAME=sys_mon
SCRIPT_DIR=$(dirname "$0")
WORKSPACE_DIR=$(realpath "$SCRIPT_DIR/..")
APP_SOURCE_PATH=$WORKSPACE_DIR/build/$APP_NAME
APP_TARGET_PATH=/usrdata/bin
# JSON_FILE=$WORKSPACE_DIR/cfg/smart_antenna.json
# JSON_TARGET_PATH=/usrdata
SSH_OPTIONS="-o IdentitiesOnly=yes"

echo "====================================="
echo "        Updating application         "
echo "====================================="
echo "Stopping sys_mon.service..."
sshpass -p $PASSWORD ssh $SSH_OPTIONS $USER@$HOST "systemctl stop sys_mon.service"

# Send application to target
echo "Sending $APP_NAME to target..."
sshpass -p $PASSWORD scp $SSH_OPTIONS $APP_SOURCE_PATH $USER@$HOST:$APP_TARGET_PATH

# # Copy JSON file to target
# echo "Copying $JSON_FILE to target..."
# sshpass -p $PASSWORD scp $SSH_OPTIONS $JSON_FILE $USER@$HOST:$JSON_TARGET_PATH

echo "Starting sys_mon.service..."
sshpass -p $PASSWORD ssh $SSH_OPTIONS $USER@$HOST "systemctl start sys_mon.service"

echo "Done."
echo "====================================="
echo "        Application updated          "
echo