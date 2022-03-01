# Goals

- Install and configure DHCP server to run on a specific ethernet interface
- Configure network subnet to use for DHCP
- Enable DHCP server to run on boot
- Test with client device


# Steps

> Adapted from: https://computingforgeeks.com/install-and-configure-dnsmasq-on-ubuntu/

```
# We need ifconfigu for configuring a static IP address on the interface we will listen for DHCP requests
# This needs to be done on every boot **before** dnsmasq can run

sudo apt install net-tools

# Configure static IP address
sudo ifconfig enp0s25 192.168.10.1

# Assign static IP on boot (& start camera API)
# Create an executable script that will be run on boot
sudo cp accumen.sh /usr/local/bin
sudo chmod +x /usr/local/bin/accumen.sh

# Create a systemd service to launch on boot
sudo cp accumen.service /etc/systemd/system/

# Enable service to start on boot
sudo systemctl enable accumen.service

# Install dnsmasq - this will also autostart the dnsmasq service and will fail as there's already a DNS server running as part of NetworkManager
sudo apt install dnsmasq

# Copy our edited config file
sudo cp dnsmasq.conf /etc/dnsmasq.conf

# Restart dnsmasq
sudo systemctl restart dnsmasq.service

# Reboot to check everything worked ok
sudo reboot

```
