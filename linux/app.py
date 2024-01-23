"""
Flow:
- Initialise camera
- Start background thread for barcode scanning
- Start flask app to listen for triggers
- 
"""
import logging
import json
import typer
import flask
import semver
from os import path, getcwd
from camera.camera import initialise_camera, capture_optimised
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
g_led_rgb = {255, 255, 255}

g_path = "/tmp"

g_enable_single_color_rejection = True
g_enable_brightness_optimisation = True
g_enable_hue_optimisation = False
g_enable_contrast_optimisation = True

g_brightness_optimal = 37
g_brightness_diff = 2
g_exposure_auto = False
g_exposure_absolute = 300
g_exposure_absolute_min = 25
g_exposure_absolute_max = 1000
g_exposure_absolute_step = 25
brightness_slope = 1
brightness_intercept = 0
best_brightness_diff = 1E10
best_exposure = 0

g_hue_min = 80
g_hue_max = 150

g_contrast_optimal = 30
g_contrast_diff = 3
g_contrast_control = 32
g_contrast_control_min = 32
g_contrast_control_max = 48
g_contrast_control_step = 2

g_max_attempts = 50



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
    max_attempts: int = g_max_attempts,
    brightness_optimal: int = g_brightness_optimal,
    brightness_diff: int = g_brightness_diff,
    enable_single_color_rejection: bool = g_enable_single_color_rejection,
    enable_brightness_optimisation: bool = g_enable_brightness_optimisation,
    enable_hue_optimisation: bool = g_enable_hue_optimisation,
    enable_contrast_optimisation: bool = g_enable_contrast_optimisation,
    path: str = g_path,
    hue_min: int = g_hue_min,
    hue_max: int = g_hue_max,
    contrast_optimal: int = g_contrast_optimal,
    contrast_diff: int = g_contrast_diff,
    contrast_control_min: int = g_contrast_control_min,
    contrast_control_max: int = g_contrast_control_max,
    contrast_control_step: int = g_contrast_control_step,
    exposure_absolute_min: int = g_exposure_absolute_min,
    exposure_absolute_max: int = g_exposure_absolute_max,
    exposure_absolute_step: int = g_exposure_absolute_step,
    exposure_auto: bool = g_exposure_auto,
    red: int = 255,
    green: int = 255,
    blue: int = 255,
):
    global cam
    global stream
    global exiting
    global g_led_rgb
    g_led_rgb = {red, green, blue}
    logging.basicConfig(
        level=logging.DEBUG,
        filename=logfile,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    )
    cam, stream = initialise_camera(
        device=device, 
        path=path,
        skip=skip,
        max_attempts=max_attempts,
        brightness_optimal=brightness_optimal,
        brightness_diff=brightness_diff,
        enable_single_color_rejection=enable_single_color_rejection,
        enable_brightness_optimisation=enable_brightness_optimisation,
        enable_hue_optimisation=enable_hue_optimisation,
        enable_contrast_optimisation=enable_contrast_optimisation,
        hue_min=hue_min,
        hue_max=hue_max,
        contrast_optimal=contrast_optimal,
        contrast_diff=contrast_diff,
        contrast_control_min=contrast_control_min,
        contrast_control_max=contrast_control_max,
        contrast_control_step=contrast_control_step,
        exposure_absolute_min=exposure_absolute_min,
        exposure_absolute_max=exposure_absolute_max,
        exposure_absolute_step=exposure_absolute_step,
        exposure_auto=exposure_auto
    )
    try:
        init_service(host, port)
    except Exception as e:
        print("Exception in mdns.init_service:", str(e))
    app.run(host=host, port=port, debug=False)
    stream.close()
    cam.close()


if __name__ == "__main__":
    typer.run(main)
