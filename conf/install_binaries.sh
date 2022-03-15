#!/bin/bash

sudo apt install -y wget python3-pip

sudo pip3 install adafruit-ampy esptool

sudo wget https://iotready-public.s3.ap-south-1.amazonaws.com/aira/accumen_junior/barcode.bin -O /usr/local/bin/barcode.bin

sudo chmod +x /usr/local/bin/barcode.bin

sudo wget https://iotready-public.s3.ap-south-1.amazonaws.com/aira/accumen_junior/camera.bin -O /usr/local/bin/camera.bin

sudo chmod +x /usr/local/bin/camera.bin

sudo cp accumen.sh /usr/local/bin/
