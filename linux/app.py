"""
Flow:
- Initialise camera
- Start background thread for barcode scanning
- Start flask app to listen for triggers
- 
"""
import logging
import typer
import uvicorn
import semver
from os import path, getcwd
from pydantic import BaseModel
from camera.camera import initialise_camera, capture_optimised
from api_client import validate_image
from fastapi import FastAPI
from fastapi.responses import FileResponse

cam = None
stream = None
exiting = False
firmware_fname = "firmware.py"
firmware_version = None

log = logging.getLogger(__name__)

LOG_LEVELS = {
    "info": log.info,
    "debug": log.debug,
    "warn": log.warn,
    "error": log.error,
}


class Log(BaseModel):
    level: str
    text: str


try:
    with open(firmware_fname, "r") as f:
        for line in f.readlines():
            if line.startswith("VERSION"):
                firmware_version = line.strip().split("VERSION = ")[-1].replace('"', "")
                break
    print("firmware_version", firmware_version)
    assert firmware_version, "Invalid firmware version"
except Exception as e:
    print(str(e))

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
        validate_image(fpath)
        return ret


@app.get("/ota")
def check_ota(version: str):
    print("Device firmware version:", version)
    print("Current firmware version:", firmware_version)
    if version and firmware_version and semver.compare(firmware_version, version) > 0:
        return {"version": firmware_version, "fpath": f"/files/{firmware_fname}"}
    else:
        return {}


@app.get(f"/files/{firmware_fname}")
def download_ota_file():
    return FileResponse(
        path=getcwd() + "/" + firmware_fname,
        media_type="application/octet-stream",
        filename=firmware_fname,
    )


@app.post("/log")
def firmware_log(item: Log):
    f = LOG_LEVELS.get(item.level) or log.debug
    f("FW: " + item.text)
    return True


def main(
    device: int = 0,
    path: str = "/tmp",
    logfile: str = "accumen_junior.log",
):
    global cam
    global stream
    global exiting
    logging.basicConfig(
        level=logging.DEBUG,
        filename=logfile,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    )
    cam, stream = initialise_camera(device=device, path=path)
    uvicorn.run(app, host="0.0.0.0", port=8000)
    stream.close()
    cam.close()


if __name__ == "__main__":
    typer.run(main)
