from camera import initialise_camera, capture_optimised
from time import sleep

cam, stream = initialise_camera()

while True:
    capture_optimised(cam, stream)
    sleep(10)
