import os
import logging
import ids_peak.ids_peak as ids_peak
from io import BytesIO
from datetime import datetime
from subprocess import call
from PIL import Image, ImageStat
from ids_peak_ipl import ids_peak_ipl as ids_ipl
import tkinter as tk
from time import sleep


filename = 0
preview_image = None
g_width = 3264
g_height = 2448
g_xoffset = 408
g_yoffset = 0
g_path = "/tmp"

def initialise_camera(device_sel):
    #init library
    try:
        ids_peak.Library.Initialize()
        device_manager = ids_peak.DeviceManager.Instance()
        device_manager.Update()
        device_descriptors = device_manager.Devices()

        print("Found Devices: " + str(len(device_descriptors)))
        for device_descriptor in device_descriptors:
            print(device_descriptor.DisplayName())

        device = device_descriptors[device_sel].OpenDevice(ids_peak.DeviceAccessType_Exclusive)
        print("Opened Device: " + device.DisplayName())
        remote_device_nodemap = device.RemoteDevice().NodeMaps()[0]
    except Exception as e:
        print(f"IDS Camera Init Error: {e}")
    #software trigger
    remote_device_nodemap.FindNode("TriggerSelector").SetCurrentEntry("ExposureStart")
    remote_device_nodemap.FindNode("TriggerSource").SetCurrentEntry("Software")
    remote_device_nodemap.FindNode("TriggerMode").SetCurrentEntry("On")


    #set camera cofiguration

    #read node value
    try:
        exposure_time = remote_device_nodemap.FindNode("ExposureTime").Value()
        print(f"Exposure Time:        {exposure_time}")

        # Get exposure range. All values in microseconds
        min_exposure_time = remote_device_nodemap.FindNode("ExposureTime").Minimum()
        max_exposure_time = remote_device_nodemap.FindNode("ExposureTime").Maximum()
        print(f"Minimumposure:  {min_exposure_time}     Maximumposure: {max_exposure_time}")

        # change exposure
        remote_device_nodemap.FindNode("ExposureTime").SetValue(500000) # in microseconds

    except Exception as e:
        print(f"Exposure set error: {str(e)}")
    # remote_device_nodemap.FindNode("ExposureTime").SetValue(100000) # in microseconds
    
    ## Gain  
    # Before accessing Gain, make sure GainSelector is set correctly
    # Set GainSelector to "AnalogAll" (str)
    remote_device_nodemap.FindNode("GainSelector").SetCurrentEntry("AnalogAll")
    # Determine the current Gain (float)
    value = remote_device_nodemap.FindNode("Gain").Value()
    # Set Gain to 1.0 (float)
    remote_device_nodemap.FindNode("Gain").SetValue(1.0)

    ##BlackLevel
    # Before accessing BlackLevel, make sure PixelFormat is set correctly
    # Set PixelFormat to "Mono8" (str)
    # remote_device_nodemap.FindNode("PixelFormat").SetCurrentEntry("Mono8")
    # # Determine the current BlackLevel (float)
    # value = remote_device_nodemap.FindNode("BlackLevel").Value()
    # # Set BlackLevel to 1.0 (float)
    # remote_device_nodemap.FindNode("BlackLevel").SetValue(1.0)
    
    ## ROI (image crop)
    # .Minimum(), .Maximum(), .Increment()
    x = 0
    y = 0
    width = 4000
    height = 3000
    remote_device_nodemap.FindNode("OffsetX").SetValue(x)
    remote_device_nodemap.FindNode("OffsetY").SetValue(y)
    remote_device_nodemap.FindNode("Width").SetValue(width)
    remote_device_nodemap.FindNode("Height").SetValue(height)

    
    
    ## brightness
    # manual brightness control
    #       ExposureTime = 300;GainSelector = "AnalogAll";Gain = 8.0
    # auto brightness control
    ExposureAuto = "Continuous"
    GainAuto = "Continuous"
    remote_device_nodemap.FindNode("ExposureAuto").SetCurrentEntry(ExposureAuto)
    remote_device_nodemap.FindNode("GainAuto").SetCurrentEntry(GainAuto)

    ## white balance (auto)
    remote_device_nodemap.FindNode("BalanceWhiteAuto").SetCurrentEntry("Continuous")

    # hue
    # contrast
    return device,remote_device_nodemap

def capture_optimised(device,remote_device_nodemap):
    
    try:
        ret = {}
        count = 0
        #start image acquisition
        datastream = device.DataStreams()[0].OpenDataStream()
        payload_size = remote_device_nodemap.FindNode("PayloadSize").Value()
        for i in range(datastream.NumBuffersAnnouncedMinRequired()):
            buffer = datastream.AllocAndAnnounceBuffer(payload_size)
            datastream.QueueBuffer(buffer)

        datastream.StartAcquisition()
        remote_device_nodemap.FindNode("AcquisitionStart").Execute()
        remote_device_nodemap.FindNode("AcquisitionStart").WaitUntilDone()

        ##image grab ##

        # trigger image
        remote_device_nodemap.FindNode("TriggerSoftware").Execute()
        buffer = datastream.WaitForFinishedBuffer(1000)
        # convert to RGB
        raw_image = ids_ipl.Image.CreateFromSizeAndBuffer(buffer.PixelFormat(),
                                                                                buffer.BasePtr(),
                                                                                buffer.Size(),
                                                                                buffer.Width(),
                                                                                buffer.Height())
        color_image = raw_image.ConvertTo(ids_ipl.PixelFormatName_RGB8)
        datastream.QueueBuffer(buffer)
        datastream.StopAcquisition()
        # remote_device_nodemap.FindNode("ExposureMode").SetCurrentEntry("Continuous")
        now = int(datetime.now().timestamp())
        tmppath_png = f"/tmp/{now}.png"
        tmppath= f"/tmp/{now}.jpg"
        fpath = f"{g_path}/{now}.jpg"
        #save image here
        picture = color_image.get_numpy_3D() 
        image = Image.fromarray(picture) # Convert the image data to a PIL Image object
        image.save(tmppath_png,'PNG')
        # Only needed until we figure out how to use crop directly within ids_peak 
        call(f"convert {tmppath_png} -crop {g_width-2*g_xoffset}x{g_height-2*g_yoffset}+{g_xoffset}+{g_yoffset} {fpath}", shell=True)
        call(f"convert {tmppath_png} {tmppath}")
        ret['path'] = fpath
        ret['attempts'] = count + 1
        ret['image_obj'] = image
        ret['image'] = color_image #ids_peak image byte
        return ret
    except Exception as e:
        print(str(e))
        return {"error": str(e)}

def save_image(image_byte, filename):

    ## try this out 
    #with open('captured_image.png', 'wb') as f:
    #    f.write(image_byte.tobytes())
    
    # global f_name
    picture = image_byte.get_numpy_3D()

    # Convert the image data to a PIL Image object
    image = Image.fromarray(picture)
    # Save the image as a PNG file
    f_name = f'pd{filename}.png'
    image.save(f_name,'PNG')
    print(f"Done saving pd{filename}.png")
    
def close_cam():
    ids_peak.Library.Close()

if __name__ == '__main__':
    device_sel = 0
    dev,nmap = initialise_camera(device_sel)
    img = capture_optimised(dev,nmap)
    #save_image(img)
   
