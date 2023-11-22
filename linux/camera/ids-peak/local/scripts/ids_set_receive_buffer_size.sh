#!/usr/bin/env sh

check_and_set_value() {
    option=$1
    new_value=$2
    sysctl_file="/etc/sysctl.conf"

    akt_value=$(grep "^$option" ${sysctl_file} | cut -d'=' -f2)

    if [ -z "$akt_value" ]; then
        # Value not present
        if echo "$option=$new_value" >> $sysctl_file; then
            echo "Successfully set $option to $new_value."
            reload_changes=true
        else
            echo "Could not set $option to $new_value in $sysctl_file." >&2
        fi
    elif [ "$akt_value" -eq "$new_value" ]; then
        # Value already set correctly
        echo "$option already set $new_value."
    else
        echo "$option already configured to $akt_value."
        if [ "$akt_value" -lt "$new_value" ]; then
            # Value is less then required
            manual_changes=true
            echo "This is less then required."
            echo "Please change the entry $option manually to:"
            echo " \"$option=$new_value\" in $sysctl_file."
        else
            # Value is greater then required
            echo "Configured value is already greater then required."
        fi
    fi

    echo 
}


# Check if called as root
if [ $(id -u) -ne 0 ]; then
    echo "Please run as root" >&2
    exit 1
else
    reload_changes=false
    manual_changes=false

    check_and_set_value net.core.rmem_max 26214400
    check_and_set_value net.core.rmem_default 26214400

    if $reload_changes; then
        sysctl --system
    fi

    if $manual_changes; then
        echo "After changing parameters manually call"
        echo "\"sysctl --system\" to reload it."
    fi
fi
