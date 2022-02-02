"""
Flow:
- Initialise camera
- Start background thread for barcode scanning
- Subscribe to Redis for barcode events and publish to AIRA API once message received
- Subscribe to Redis for controller events (limit switch) and trigger camera once message received
- 
"""
import threading
import json
import time
import redis
import typer
from datetime import datetime
# from barcode.scan import read_all_scanners
from camera.camera import initialise_camera, capture_optimised

g_redis_host = '192.168.10.1'
g_redis_port = 6379
g_redis = None
g_pubsub = None
io_channel = 'remote_io'
barcode_channel = 'barcode'

limit_switch_pin = 36

cam = None
stream = None
exiting = False


def barcode_event(message):
    barcode = json.loads(message.get('data'))
    print("Barcode received via Redis:", barcode)
    # Dispatch to AIRA's barcode validation API - WIP


def io_event(message):
    inputs = json.loads(message.get('data'))
    print(inputs)
    if inputs.get(str(limit_switch_pin)) == 0:
        trigger()


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

def healthcheck():
    return {"ok": True, "now": datetime.now().timestamp()}

def trigger():
    
    return capture_optimised(cam, stream)

def main(
    device: int = 0,
):
    global cam
    global stream
    global exiting
    cam, stream = initialise_camera(device=device)
    while True:
        get_message()
        time.sleep(0.1)
    stream.close()
    cam.close()
    

if __name__=="__main__":
    typer.run(main)
