import typer
from camera.camera import initialise_camera, capture_optimised
from time import sleep

def main(
    device: int = 0,
    count: int = 50,
    path: str = "/tmp",
    delay: int = 10,
):
    global cam
    global stream
    cam, stream = initialise_camera(device=device, path=path)
    for i in range(count):
        print(capture_optimised(cam, stream))
        sleep(delay)
    

if __name__=="__main__":
    typer.run(main)
