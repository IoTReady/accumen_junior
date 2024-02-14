"""
Flow:
- Initialise camera
- Start background thread for barcode scanning
- Start flask app to listen for triggers
- 
"""
import os

# Set the PEAK_PATH environment variable
peak_path = "/home/ccmsadmin/accumen/ids-peak_2.6.1.0-16200_amd64/"
os.environ["PEAK_PATH"] = peak_path

# Set the LD_LIBRARY_PATH environment variable
lib_path = "/home/ccmsadmin/accumen/ids-peak_2.6.1.0-16200_amd64/lib/"
os.environ["LD_LIBRARY_PATH"] = lib_path

# Set the GENICAM_GENTL32_PATH and GENICAM_GENTL64_PATH environment variables
genicam_path = os.path.join(peak_path, "lib", "ids", "cti")
os.environ["GENICAM_GENTL32_PATH"] = f"{genicam_path}:{os.environ.get('GENICAM_GENTL32_PATH', '')}"
os.environ["GENICAM_GENTL64_PATH"] = f"{genicam_path}:{os.environ.get('GENICAM_GENTL64_PATH', '')}"



# Print the environment variables (optional)
print("PEAK_PATH:", os.environ.get("PEAK_PATH"))
print("LD_LIBRARY_PATH:", os.environ.get("LD_LIBRARY_PATH"))
print("GENICAM_GENTL32_PATH:", os.environ.get("GENICAM_GENTL32_PATH"))
print("GENICAM_GENTL64_PATH:", os.environ.get("GENICAM_GENTL64_PATH"))




import logging
import json
import typer
import flask
import semver
from os import path, getcwd
from camera.camera_ids_cli import close_cam, initialise_camera, capture_optimised
from api_client import validate_image
from mdns import init_service

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

# RGB values for the LED ring
g_led_rgb = (255, 255, 255)

g_path = "/tmp"




try:
    with open("/tmp/" + firmware_fname, "r") as f:
        for line in f.readlines():
            if line.startswith("VERSION"):
                firmware_version = line.strip().split("VERSION = ")[-1].replace('"', "")
                break
    print("firmware_version", firmware_version)
    assert firmware_version, "Invalid firmware version"
except Exception as e:
    print(str(e))

app = flask.Flask(__name__)


@app.get("/")
async def healthcheck():
    return {"ok": True}


@app.post("/")
def trigger():
    # The AIRA API expects a path wrt to the Docker container so we need to remap.
    # fpath_for_api = "/mnt/original_image/"
    print("Triggered")
    ret = capture_optimised(cam, stream)
    if ret.get("error"):
        return flask.Response(json.dumps(ret), status=503, mimetype="application/json")
    else:
        print("Captured:", ret)
        # fname = path.basename(ret.get("path"))
        # fpath = path.join(fpath_for_api, fname)
        fpath = ret.get("path")
        validate_image(fpath)
        return flask.Response(json.dumps(ret), status=200, mimetype="application/json")


@app.get("/ota")
def check_ota():
    version = flask.request.args.get("version")
    print("Device firmware version:", version)
    print("Current firmware version:", firmware_version)
    if version and firmware_version and semver.compare(firmware_version, version) > 0:
        return {"version": firmware_version, "fpath": f"/files/{firmware_fname}"}
    else:
        return {}


@app.get(f"/files/{firmware_fname}")
def download_ota_file():
    return flask.send_file(
        path_or_file="/tmp/" + firmware_fname,
        mimetype="application/octet-stream",
    )


@app.get("/rgb")
def get_rgb():
    red, green, blue = g_led_rgb
    response_json = {"red": red, "green": green, "blue": blue}
    return flask.Response(json.dumps(response_json), status=200, mimetype="application/json")


@app.post("/logs")
@app.post("/log")
def firmware_log():
    item = flask.request.json
    if item:
        f = LOG_LEVELS.get(item.get("level")) or log.debug
        f("FW: " + item.get("text") or "")
    return {"ok": True}


def main(
    device: int = 0,
    logfile: str = "accumen_junior.log",
    host: str = "0.0.0.0",
    port: int = 8000,
    skip: int = 2,
):
    global cam
    global stream
    global exiting
    global g_led_rgb 
    g_led_rgb = (red, green, blue)
    logging.basicConfig(
        level=logging.DEBUG,
        filename=logfile,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    )
    #cam, stream = initialise_camera(
    #    device=device, 
    #    path=path,
    #    skip=skip,
    #)
    cam, stream = initialise_camera(device) ## add device sel. control
    try:
        init_service(host, port)
    except Exception as e:
        print("Exception in mdns.init_service:", str(e))
    app.run(host=host, port=port, debug=False)
    #stream.close()
    #cam.close()
    #close camera and library cleanly
    close_cam()


if __name__ == "__main__":
    typer.run(main)
