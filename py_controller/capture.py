import typer
from camera.camera import initialise_camera, capture_optimised
from time import sleep

def main(
    device: int = 0,
):
    global cam
    global stream
    cam, stream = initialise_camera(device=device)
    while True:
        capture_optimised(cam, stream)
        sleep(10)
    

if __name__=="__main__":
    typer.run(main)
