#!/bin/sh

BASEDIR=/home/ccmsadmin/accumen/accumen_junior/
# Set static IP address
sudo ifconfig ens8191 192.168.10.1

# Warm up camera
v4l2-ctl -c exposure_auto=3
fswebcam -D 3 -S 2 -r 3264x2448 -F 3

# Start Camera API
echo "Starting Camera API"
$BASEDIR/py_controller/run_camera.sh &

# Wait for network
#while ! ping -c 1 -W 1 172.16.61.180; do
#    echo "Waiting for ping response - network interface might be down..."
#    sleep 1
#done

# Mount Windows Share
#sudo mount /mnt/ccmsimg

# Start Barcode Scanner
echo "Starting barcode scanner"
$BASEDIR/py_controller/run_barcode.sh &

# Move files
#echo "Starting file mover"
#python3 /home/ccmsadmin/Documents/fetch_image/fetch_image_code/trial.py | tee /var/log/trial_py.log &
