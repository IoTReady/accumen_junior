import os
import cProfile
import sys
from asyncio import sleep
from numpy.core.fromnumeric import sort
from pypylon import pylon
from time import sleep
from datetime import datetime
from PIL import Image

g_path = "/tmp"
EXPOSURE = 40000 
GAIN = 1
GAMMA = 3
WIDTH = 4096
HEIGHT = 3000
BALANCE_RATIO_SELECTOR_GREEN = 1.0
BALANCE_RATIO_SELECTOR_RED = 1.0
BALANCE_RATIO_SELECTOR_BLUE = 1.0
OFFSET_X = 0 
OFFSET_Y = 0 

def initialise_camera(
        device_sel,
        exposure= EXPOSURE,
        gain= GAIN,
        gamma= GAMMA,
        wb_green = BALANCE_RATIO_SELECTOR_GREEN,
        wb_red = BALANCE_RATIO_SELECTOR_RED,
        wb_blue = BALANCE_RATIO_SELECTOR_BLUE,
        offsetX = OFFSET_X,
        offsetY = OFFSET_Y,
        width = WIDTH,
        height = HEIGHT
        ):
    try:
        # Create an instant camera object for the first available camera
        camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice()) 
        camera_info = pylon.InstantCamera(pylon.TlFactory.GetInstance().EnumerateDevices()) 
        print(f"camera_info: {camera_info}")
        img_res = pylon.PylonImage()
        # Open the camera
        camera.Open()
        
        # https://docs.baslerweb.com/gamma#python
        camera.GainAuto.SetValue("Off")
        camera.ExposureTime.SetValue(exposure)  # 10,000 microseconds (10 ms) 
        camera.Gain.SetValue(gain)  # Set gain to 6 dB
        camera.Gamma.SetValue(gamma) # Gamma < 1: The overall brightness increases.

        camera.Width.SetValue(width)
        camera.OffsetX.SetValue(offsetX);
        camera.Height.SetValue(height)
        camera.OffsetY.SetValue(offsetY);

        #white balance
        camera.BalanceRatioSelector.SetValue('BalanceRatioSelector_Green');
        camera.BalanceRatioAbs.SetValue(wb_green);
        camera.BalanceRatioSelector.SetValue('BalanceRatioSelector_Red');
        camera.BalanceRatioAbs.SetValue(wb_red);
        camera.BalanceRatioSelector.SetValue('BalanceRatioSelector_Blue');
        camera.BalanceRatioAbs.SetValue(wb_blue);
        camera.BalanceWhiteAuto.SetValue('Off');

        return camera, img_res
    except Exception as e:
        print(f"Error initializing camera: {e}")
        return None, None

def capture_frame(camera):
    try:
        # Grab one image
        camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly)
        print("Started Grabbing")
        img = camera.RetrieveResult(10000, pylon.TimeoutHandling_ThrowException)
        print("Waiting on Image capture")
        return img
    except Exception as e:
        print(f"Error capturing frame: {e}")
        return e

def save_image(camera,img, img_res):
    try:
        now = int(datetime.now().timestamp())
        tmppath = f"/tmp/{now}.tiff"
        tmppath_png = f"/tmp/{now}.png"
        tmppath_jpg = f"/tmp/{now}.jpg"
        fpath = f"{g_path}/{now}.jpg"
        if img is not None and img.GrabSucceeded():

            img_res.AttachGrabResultBuffer(img)
            print("Attached Grab results ...")
            img_res.Save(pylon.ImageFileFormat_Tiff, tmppath)
            
            # Convert the TIFF image to PNG using Pillow
            #tiff_image = Image.open(tmppath)
            #tiff_image.save(tmppath_png, "PNG")
            #print(f"Image Saved {tmppath_png}")

            tiff_image = Image.open(tmppath).convert("RGB")
            tiff_image.save(fpath,"JPEG",quality=100)
            print(f"Image Saved {tmppath_jpg}")

            # Release the image
            img.Release()
            print("Trying to release the image")
        sleep(1)
        if camera.IsGrabbing():
            camera.StopGrabbing()
        img = capture_frame(camera)
        return fpath, img_res 
    except Exception as e:
        print(f"Error saving image: {e}")
        return e, img_res

def capture_optimised(camera, img_res, path = g_path):
    global g_path
    g_path = path
    assert os.path.exists(g_path), f"Directory '{g_path}' does not exist"
    try:
        ret = {}
        count = 0
        img = capture_frame(camera)
        ERROR_MSG = None
        try:
            g_path, img= save_image(camera, img, img_res)
        except Exception as e:
            ERROR_MSG = e
        ret['path'] = g_path
        ret['attempts'] = count + 1
        ret['image_obj'] = img_res
        ret['image'] = img #ids_peak image byte
        return ret
    except Exception as e:
        print(str(e))
        return {"error": str(e)}

def camera_close(camera):
    try:
        # Release the image and stop grabbing
        if camera.IsGrabbing():
            camera.StopGrabbing()
        sleep(3)        
        # Close the camera
        camera.Close()
    except Exception as e:
        print(f"Error closing camera: {e}")

if __name__ == "__main__":
    #cProfile.run("camera, img_res = init_camera()", sort='cumulative')
    #cProfile.run("img = capture_frame(camera)", sort='cumulative')
    #cProfile.run("save_image(camera, img, img_res)", sort='cumulative')
    #cProfile.run("camera_close(camera)", sort='cumulative')
    camera, img_res = initialise_camera(EXPOSURE, GAMMA, GAIN)
    
    if camera:
        ret = capture_optimised(camera,img_res)
        #cProfile.run("save_image(camera, img, img_res)",sort='cumulative')
        camera_close(camera)

