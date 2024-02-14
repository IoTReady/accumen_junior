#!/usr/bin/env sh

CURRENT_PATH=$(dirname "$0")

if [ $(id -u) -ne 0 ]; then
    echo "Please run as root" >&2
    exit 1
fi

if command -v xdg-mime; then
    update-mime-database /usr/share/mime || true
    update-icon-caches /usr/share/icons/hicolor || true
    xdg-mime default /usr/share/application/ids-guf.desktop application/x-ids-guf-firmware
else
    echo "xdg_utils missing! Please install them e.g. 'apt install xdg_utils' " >&2
    exit 1
fi
