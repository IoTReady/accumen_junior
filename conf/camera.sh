#!/bin/sh

BASEDIR=/usr/local/bin
# Set static IP address
sudo ifconfig ens4f0 192.168.10.1

# Warm up camera
v4l2-ctl -c exposure_auto=3
fswebcam -D 3 -S 2 -r 3264x2448 -F 3

# Start Camera API
echo "Starting Camera API"
$BASEDIR/camera.bin 


