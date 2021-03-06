#!/bin/bash

sudo systemctl disable accumen.service
sudo rm /etc/systemd/system/accumen.service
sudo rm /usr/local/bin/accumen.sh

sudo cp interfaces /etc/network/

sudo apt install -y wget python3-pip
sudo pip3 install adafruit-ampy esptool

sudo systemctl stop barcode
sudo pkill barcode
sudo wget https://iotready-public.s3.ap-south-1.amazonaws.com/aira/accumen_junior/barcode.bin -O /usr/local/bin/barcode.bin
sudo chmod +x /usr/local/bin/barcode.bin
sudo cp barcode.service /etc/systemd/system/
sudo systemctl enable barcode.service

sudo systemctl stop camera
sudo pkill camera
sudo wget https://iotready-public.s3.ap-south-1.amazonaws.com/aira/accumen_junior/camera.bin -O /usr/local/bin/camera.bin
sudo chmod +x /usr/local/bin/camera.bin
sudo cp camera.sh /usr/local/bin/
sudo cp camera.service /etc/systemd/system/
sudo systemctl enable camera.service


sudo gpasswd -a ccmsadmin dialout
sudo gpasswd -a ccmsadmin uucp 
sudo gpasswd -a ccmsadmin video
