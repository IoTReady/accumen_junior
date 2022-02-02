"""
Flow:
- Initialise camera
- Start background thread for barcode scanning
- Subscribe to Redis for barcode events and publish to AIRA API once message received
- Subscribe to Redis for controller events (limit switch) and trigger camera once message received
- 
"""
import asyncio
import threading
import json
import time
import redis
import typer
import uvicorn
from datetime import datetime
from fastapi import FastAPI
# from barcode.scan import read_all_scanners
from camera.camera import initialise_camera, capture_optimised

g_redis_host = 'localhost'
g_redis_port = 6379
g_redis = None
g_pubsub = None
barcode_channel = 'barcode'

cam = None
stream = None
exiting = False

app = FastAPI()

def barcode_event(message):
    barcode = json.loads(message.get('data'))
    print(barcode)
    # Dispatch to AIRA's barcode validation API - WIP


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
        print("Subscribed!")
        return True
    except:
        time.sleep(1)
        return open_redis()

def barcode_monitor():
    while not exiting:
        get_message()
        time.sleep(0.1)

@app.get("/")
def healthcheck():
    return {"ok": True, "now": datetime.now().timestamp()}

@app.post('/')
def trigger():
    return capture_optimised(cam, stream)

def main(
    device: int = 0,
):
    global cam
    global stream
    global exiting
    cam, stream = initialise_camera(device=device)
    # barcode_scanner_thread = threading.Thread(target=read_all_scanners)
    # barcode_scanner_thread.start()
    if open_redis():
        barcode_monitor_thread = threading.Thread(target=barcode_monitor)
        barcode_monitor_thread.start()
    uvicorn.run(app)
    stream.close()
    cam.close()
    exiting = True
    

if __name__=="__main__":
    typer.run(main)
