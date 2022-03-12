"""
Flow:
- Initialise camera
- Start background thread for barcode scanning
- Start flask app to listen for triggers
- 
"""
import time
import typer
import uvicorn
from os import path
from camera.camera import initialise_camera, capture_optimised
from api_client import validate_image
from fastapi import FastAPI

cam = None
stream = None
exiting = False



app = FastAPI()

@app.get("/")
async def healthcheck():
    return {"ok": True}

@app.post("/")
def trigger():
    # The AIRA API expects a path wrt to the Docker container so we need to remap.
    fpath_for_api = "/mnt/original_image/"
    print("Triggered")
    # Trigger Status: 0 = In Progress; 1 = Done; 2 = Error
    ret = capture_optimised(cam, stream)
    if ret.get("error"):
        print(ret)
        raise Exception("Capture error")
    else:
        print("Captured:", ret)
        fname = path.basename(ret.get("path"))
        fpath = path.join(fpath_for_api, fname)
        #validate_image(fpath)
        return ret

def main(
    device: int = 0,
    path: str = "/tmp",
):
    global cam
    global stream
    global exiting
    cam, stream = initialise_camera(device=device, path=path)
    uvicorn.run(app, host="0.0.0.0", port=8000)
    stream.close()
    cam.close()
    

if __name__=="__main__":
    typer.run(main)
