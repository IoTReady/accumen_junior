#!/usr/bin/env sh

# Check if called as root
if [ $(id -u) -ne 0 ]; then
    echo "Please run as root" >&2
    exit 1
else
    if [ -f "/etc/udev/rules.d/99-ids-usb-access.rules" ]
    then
        rm -rf "/etc/udev/rules.d/99-ids-usb-access.rules"
        #reload udev rules
        udevadm control --reload-rules
        udevadm trigger
        echo "Successfully removed udev rule for USB3 gentl"
    else
        echo "Udev rule for USB3 gentl not installed" >&2
    fi
fi
