# Prints/dispatches barcodes captured by HID type Barcode Scanners (typically USB).
# Uses evdev to get exclusive access to the devices (hence must be run as root).
# Can read multiple devices simultaneously.
# 
# Author: Tej Pochiraju (IoTReady.co)
# Date: 23/12/2021
# UNLICENSED

import time
import asyncio
import evdev
import redis

# Change this if not using a Zebra/Symbol/Motorola scanner.
DEVICE_PREFIX = "Symbol Technologies, Inc, 2008 Symbol Bar Code Scanner::EA"
ENTER_KEYCODE = 28
TAB_KEYCODE = 15
SHIFT_KEYCODES = [42, 54]


# Redis connection params
g_redis_host = 'localhost'
g_redis_port = 6379
g_redis = None
channel = 'barcode'


# The map below translates evdev event codes to ASCII
# First value in array is ASCII without shift, second value is with shift
key_map = {
    0: [None, None],
    1: ["ESC", "ESC"],
    2: ["1", "!"],
    3: ["2", "@"],
    4: ["3", "#"],
    5: ["4", "$"],
    6: ["5", "%"],
    7: ["6", "^"],
    8: ["7", "&"],
    9: ["8", "*"],
    10: ["9", "("],
    11: ["0", ")"],
    12: ["-", "_"],
    13: ["=", "+"],
    14: ["BKSP", "BKSP"],
    15: ["TAB", "TAB"],
    16: ["q", "Q"],
    17: ["w", "W"],
    18: ["e", "E"],
    19: ["r", "R"],
    20: ["t", "T"],
    21: ["y", "Y"],
    22: ["u", "U"],
    23: ["i", "I"],
    24: ["o", "O"],
    25: ["p", "P"],
    26: ["[", "{"],
    27: ["]", "}"],
    28: ["CRLF", "CRLF"],
    29: ["LCTRL", "LCTRL"],
    30: ["a", "A"],
    31: ["s", "S"],
    32: ["d", "D"],
    33: ["f", "F"],
    34: ["g", "G"],
    35: ["h", "H"],
    36: ["j", "J"],
    37: ["k", "K"],
    38: ["l", "L"],
    39: [";", ":"],
    40: ['"', "'"],
    41: ["`", "~"],
    42: ["LSHFT", "LSHFT"],
    43: ["\\", "|"],
    44: ["z", "Z"],
    45: ["x", "X"],
    46: ["c", "C"],
    47: ["v", "V"],
    48: ["b", "B"],
    49: ["n", "N"],
    50: ["m", "M"],
    51: [",", "<"],
    52: [".", ">"],
    53: ["/", "?"],
    54: ["RSHFT", "RSHFT"],
    56: ["LALT", "LALT"],
    57: [" ", " "],
    100: ["RALT", "RALT"],
}


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


def find_scanners():
    """
    Returns a list of opened evdev devices whose name begins with prefix. 
    """
    devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
    scanners = [device for device in devices if device.name.startswith(DEVICE_PREFIX)]
    return scanners


def dispatch_barcode(barcode):
    """
    Helper function to do something useful with the detected barcode.
    """
    # Use the barcode here by sending to an API or updating it in a shared state dictionary
    print(f"Scanned: {barcode}")
    try:
        g_redis.publish(channel, barcode)
        g_redis.set("barcode", barcode, 5)
        print("Published!")
    except:
        time.sleep(1)
        open_redis()
        # open_redis can take an indeterminate amount of time
        # Should we publish again or ignore since barcode scanning needs to be synchronous?
        g_redis.publish(channel, barcode)

async def read_scanner(scanner):
    """
    Run an infinite, asynchronous loop reading inputs from the evdev device. 
    Dispatches the detected barcode whenever an ENTER event is detected.
    Requires the scanner to return an Enter after every barcode.
    To use with TAB endings, see commment below.
    """
    barcode = ""
    is_shift_pressed = False
    # Get exclusive access
    with scanner.grab_context():
        async for event in scanner.async_read_loop():
            if event.type == evdev.ecodes.EV_KEY:
                data = evdev.categorize(event)
                if data.scancode in SHIFT_KEYCODES:
                    # keystate can be 0 (up), 1 (down) or 2 (hold)
                    is_shift_pressed = data.keystate != 0
                # Parse only when key is pressed down
                if data.keystate == 1:
                    # Uncomment the line below is using TAB ending for barcode
                    #if data.scancode == TAB_KEYCODE:
                    # Comment line below if using TAB ending for barcode
                    if data.scancode == ENTER_KEYCODE:
                        dispatch_barcode(barcode)
                        barcode = ""
                    else:
                        # key can be None if it's not in the map
                        key = key_map.get(data.scancode)
                        if key and data.scancode not in SHIFT_KEYCODES:
                            barcode += key[is_shift_pressed]


def read_all_scanners():
    """
    Open an asynchronous loop for all detected scanners. Each will independently dispatch barcode when detected.
    """
    scanners = find_scanners()
    if len(scanners) > 0:
        open_redis()
        for scanner in scanners:
            asyncio.run(read_scanner(scanner))
            # asyncio.ensure_future(read_scanner(scanner))
        # loop = asyncio.get_event_loop()
        # loop.run_forever()
    else:
        print("No scanner found")


if __name__ == "__main__":
    read_all_scanners()
