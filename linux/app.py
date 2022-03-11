"""
Flow:
- Initialise camera
- Start background thread for barcode scanning
- Subscribe to Redis for barcode events and publish to AIRA API once message received
- Subscribe to Redis for controller events (limit switch) and trigger camera once message received
- 
"""
import json
import time
import redis
import typer
import requests
from os import path
from datetime import datetime
# from barcode.scan import read_all_scanners
from camera.camera import initialise_camera, capture_optimised
from api_client import validate_barcode, validate_image

g_redis_host = '192.168.10.1'
g_redis_port = 6379
g_redis = None
g_pubsub = None
io_channel = 'remote_io'
barcode_channel = 'barcode'

remote_io_host = '192.168.10.2'
remote_io_port = 80

# on display:
status_line_num = 3

limit_switch_pin = 36
led_1 = 2
led_2 = 12

cam = None
stream = None
exiting = False

def barcode_event(message):
    barcode = json.loads(message.get('data'))
    print("Barcode received via Redis:", barcode)
    # Dispatch to AIRA's barcode validation API - WIP
    # validate_barcode(barcode)


def io_event(message):
    data = json.loads(message.get('data'))
    triggerId = data.get("triggerId")
    if data.get('capture') and triggerId:
        print(data)
        trigger(triggerId)


def get_message():
    try:
        g_pubsub.get_message()
    except:
        time.sleep(1)
        open_redis()

def open_redis():
    global g_redis
    global g_pubsub
    # Open redis connection
    g_redis = redis.Redis(host=g_redis_host, port=g_redis_port, db=0)
    try:
        # Check if redis is running, if not we call this recursively
        g_redis.ping()
        g_pubsub = g_redis.pubsub(ignore_subscribe_messages=True)
        g_pubsub.subscribe(**{barcode_channel: barcode_event})
        g_pubsub.subscribe(**{io_channel: io_event})
        print("Subscribed to barcode and remote_io!")
        return True
    except:
        time.sleep(1)
        return open_redis()

def lcd_status_print(message):
    offset = 0
    url = f"http://{remote_io_host:remote_io_port}/display/line?line={status_line_num}&offset={offset}&text={message}"
    res = requests.get(url)
    print(res.text)

def set_led_state(pin,state):
    url = f"http://{remote_io_host:remote_io_port}/output?pin={pin}&state={state}"
    res = requests.get(url)
    print(res.text)

def trigger(triggerId):
    # The AIRA API expects a path wrt to the Docker container so we need to remap.
    fpath_for_api = "/mnt/original_image/"
    print("Triggered", triggerId)
    # Trigger Status: 0 = In Progress; 1 = Done; 2 = Error
    g_redis.set(triggerId, 0)
    ret = capture_optimised(cam, stream)
    if ret.get("error"):
        print(ret)
        g_redis.set(triggerId, 2)
    else:
        print("Capture: ", triggerId, ": ", ret)
        g_redis.set(triggerId, 1)
        fname = path.basename(ret.get("path"))
        fpath = path.join(fpath_for_api, fname)
        validate_image(fpath)
    # set_led_state(led_2, 1)
    # lcd_status_print("Imaging in Progress")
    # capture_optimised(cam, stream)
    # lcd_status_print("Imaging Complete")
    # time.sleep(2)
    # lcd_status_print("Insert Petri Dish")
    # set_led_state(led_2, 0)


def main(
    device: int = 0,
    path: str = "/tmp",
):
    global cam
    global stream
    global exiting
    cam, stream = initialise_camera(device=device, path=path)
    while True:
        get_message()
        time.sleep(0.1)
    stream.close()
    cam.close()
    

if __name__=="__main__":
    typer.run(main)
