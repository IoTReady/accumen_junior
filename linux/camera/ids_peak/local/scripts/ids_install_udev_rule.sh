#!/usr/bin/env sh

# Get script path
CURRENT_PATH=$(dirname "$0")

# Check if called as root
if [ $(id -u) -ne 0 ]; then
    echo "Please run as root" >&2
    exit 1
else
    if [ ! "$(command -v udevadm)" ]; then
        echo "Udev/udevadm is not installed" >&2
        exit 1
    elif [ -f "/etc/udev/rules.d/99-ids-usb-access.rules" ]; then
        echo "Udev rule already exists"
    else
        if cp "$CURRENT_PATH"/99-ids-usb-access.rules /etc/udev/rules.d/; then
            # set correct permissions
            if chmod 644 /etc/udev/rules.d/99-ids-usb-access.rules; then
                #reload the current udev rules
                udevadm control --reload-rules
                udevadm trigger
                echo "Successfully installed udev rule" 
            else 
                echo "Could not set permissions to udev rule" >&2
                exit 1
            fi 
        else 
            echo "Could not copy udev rule file" >&2
            exit 1
        fi
    fi
fi
