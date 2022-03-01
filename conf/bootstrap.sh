# Bootstrap script to prepare a aCCumen Junior Linux Machine
# 1. Install dependencies and update system
# 2. Copy ssh keys
# 3. Clone repo
# 4. Copy config scripts and set up services
# 5. Reboot and verify
#

#!/bin/bash

echo -e "Updating system and installing dependencies"
sudo apt update && sudo apt upgrade -y && sudo apt install redis git python3-venv python3-pip imagemagick guvcview fswebcam net-tools

echo -e "Copying ssh keys"
mkdir ~/.ssh
cp id_rsa* ~/.ssh/

echo -e "Cloning Repo"
mkdir ~/accumen/
cd ~/accumen/
git clone git@github.com:IoTReady/accumen_junior.git

echo -e "Please edit the configuration files inside accumen_junior/conf before proceeding"

echo -e "Press any key once you have edited the configuration files to suit your machine."
read response

echo -e "Copying configuration scripts"
cd accumen_junior/conf/
sudo cp dnsmasq.conf /etc/dnsmasq.conf
sudo cp accumen.service /etc/systemd/system/
sudo cp accumen.sh /usr/local/bin/
sudo systemctl enable accumen.service

echo -e "Rebooting"
sleep 3
sudo reboot
