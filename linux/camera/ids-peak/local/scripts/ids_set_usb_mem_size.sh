#!/usr/bin/env sh

set -e

SHELL_SCRIPT_ROOT=/usr/local/scripts
SHELL_SCRIPT_PATH=${SHELL_SCRIPT_ROOT}/ids_set_usb_mem_size_on_boot.sh
MEM_DEFAULT_VALUE=1000
SHELL_SCRIPT_HEADER="#!/bin/sh"
USBFS_FILE="/sys/module/usbcore/parameters/usbfs_memory_mb"

SYSTEMD_FILE_CONTENT="<<EOT
[Unit]
Description=USB Memory FS size setter

[Service]
ExecStart=${SHELL_SCRIPT_PATH}
User=root
Group=root

[Install]
WantedBy=multi-user.target"

check_and_set_value() {
    new_value=$1

    akt_value=$(cat ${USBFS_FILE})

    if [ "$akt_value" -eq "$new_value" ]; then
        # Value already set correctly
        echo "already set $new_value."
        exit 0
    else
        if echo "$new_value" >> ${USBFS_FILE}; then
            echo "Successfully set to $new_value."
        else
            echo "Could not set to $new_value in ${USBFS_FILE}." >&2
            exit 1
        fi
    fi

    echo
}


# Check if called as root
if [ "$(id -u)" -ne 0 ]; then
    echo "Please run as root" >&2
    exit 1
else

    size_mb="$1"
    if [ -z "${size_mb}" ]; then
        size_mb=${MEM_DEFAULT_VALUE}
    fi
    if ! { [ -n "${size_mb}" ] && [ "${size_mb}" -eq "${size_mb}" ] 2>/dev/null; }; then
      echo "Please provide a number as size in mb."
      exit 1
    fi

    SHELL_SCRIPT_CMD="echo ${MEM_DEFAULT_VALUE} > ${USBFS_FILE}"
    SHELL_SCRIPT_CONTENT="${SHELL_SCRIPT_HEADER}\n${SHELL_SCRIPT_CMD}"

    check_and_set_value "$size_mb"

    mkdir -p ${SHELL_SCRIPT_ROOT}
    echo "${SHELL_SCRIPT_CONTENT}" >> ${SHELL_SCRIPT_PATH}
    chmod 755 ${SHELL_SCRIPT_PATH}
    if [ -d /run/systemd/system ]; then
        echo "${SYSTEMD_FILE_CONTENT}" >> /etc/systemd/system/set_usb_mem_size.service
        systemctl enable set_usb_mem_size.service
    elif [ -f "/etc/rc.local" ]; then
        echo "Please add the following content before the exit 0 in /etc/rc.local:"
        echo "${SHELL_SCRIPT_CONTENT}"
    else
        echo "Could not add the greater value for usbfs_memory_mb to boot."
        exit 1
    fi
fi
