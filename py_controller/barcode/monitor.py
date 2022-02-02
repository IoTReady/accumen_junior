# Prints/dispatches barcodes captured by HID type Barcode Scanners (typically USB).
# Uses evdev to get exclusive access to the devices (hence must be run as root).
# Can read multiple devices simultaneously.
# 
# Author: Tej Pochiraju (IoTReady.co)
# Date: 23/12/2021
# UNLICENSED

import time
import redis


# Redis connection params
g_redis_host = 'localhost'
g_redis_port = 6379
g_redis = None


def open_redis():
    global g_redis
    # Open redis connection
    g_redis = redis.Redis(host=g_redis_host, port=g_redis_port, db=0)
    try:
        # Check if redis is running, if not we call this recursively
        g_redis.ping()
        return True
    except:
        time.sleep(1)
        return open_redis()


def read_barcode():
    """
    """
    # Use the barcode here by sending to an API or updating it in a shared state dictionary
    try:
        return g_redis.get("barcode")
    except:
        time.sleep(1)
        return open_redis()


def monitor_for_barcode():
    barcode = read_barcode()
    timeout = 5
    count = 0
    while not barcode:
        count += 1
        time.sleep(1)
        barcode = read_barcode()
        if count > timeout:
            break
    print("Barcode", barcode)
    return barcode

if __name__ == "__main__":
    while True:
        monitor_for_barcode()
        time.sleep(1)
