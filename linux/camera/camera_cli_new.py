import cProfile
import sys
from asyncio import sleep
from numpy.core.fromnumeric import sort
from pypylon import pylon
from time import sleep
from datetime import datetime
from PIL import Image

gain_redux = 0
EXPOSURE = 10000
GAIN = 1


def initialise_camera():
    try:
        # Initialize the pylon runtime system (optional)
        # pylon.pylon_initialize()
        
        # Create an instant camera object for the first available camera
        camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice()) 
        img_res = pylon.PylonImage()
        # Open the camera
        camera.Open()
        
        # Set exposure time in microseconds
        camera.ExposureTime.SetValue(EXPOSURE)  # 10,000 microseconds (10 ms)
        
        # Set gain value in dB
        camera.Gain.SetValue(GAIN)  # Set gain to 6 dB
        
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
        tmppath_png = f"/tmp/png/{now}.png"
        tmppath_jpg = f"/tmp/jpg/{now}.jpg"
        if img is not None and img.GrabSucceeded():

            img_res.AttachGrabResultBuffer(img)
            print("Attached Grab results ...")
            img_res.Save(pylon.ImageFileFormat_Tiff, tmppath)
            
            # Convert the TIFF image to PNG using Pillow
            #tiff_image = Image.open(tmppath)
            #tiff_image.save(tmppath_png, "PNG")
            #print(f"Image Saved {tmppath_png}")

            tiff_image = Image.open(tmppath).convert("RGB")
            tiff_image.save(tmppath_jpg,"JPEG",quality=100)
            print(f"Image Saved {tmppath_jpg}")

            # Release the image
            img.Release()
            print("Trying to release the image")
        sleep(1)
        if camera.IsGrabbing():
            camera.StopGrabbing()
        img = capture_frame(camera)
        return tmppath 
    except Exception as e:
        print(f"Error saving image: {e}")
        return e

def capture_optimised(camera, img_res):
    try:
        img = capture_frame(camera)
        ERROR_MSG = None
        PATH = None
        try:
            PATH = save_image(camera, img, img_res)
        except Exception as e:
            ERROR_MSG = e
        ret = {
                'brightness': None,
                'hue': None,
                'contrast': None,
                'path': PATH,
                'attempts': None,
                'exposure': EXPOSURE,
                'gain': GAIN,
                'img': img,
                'error': ERROR_MSG 
            }
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
    camera, img_res = initialise_camera()
    
    if camera:
        ret = capture_optimised(camera,img_res)
        #cProfile.run("save_image(camera, img, img_res)",sort='cumulative')
        camera_close(camera)

