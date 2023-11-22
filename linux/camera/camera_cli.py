from pypylon import pylon

## Initialize the pylon runtime system
#pylon.pylon_initialize()

## Terminate the pylon runtime system
#pylon.pylon_terminate()


def init_camera():
    
    # Create an instant camera object for the first available camera
    camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
    
    # Open the camera
    camera.Open()
    
    # Set exposure time in microseconds
    camera.ExposureTime.SetValue(10000)  # 10,000 microseconds (10 ms)
    
    # Set gain value in dB
    camera.Gain.SetValue(6.0)  # Set gain to 6 dB
    return camera

def capture_frame(camera):
    # Grab one image
    camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly)
    img = camera.RetrieveResult(10000, pylon.TimeoutHandling_ThrowException)
    return camera


def save_image(img):
    
    # Check if the image acquisition was successful
    if img.GrabSucceeded():
        # Access the image data
        image_data = img.GetArray()
    
        # Save the image as a file (e.g., PNG)
        with open('captured_image.png', 'wb') as f:
            f.write(image_data.tobytes())
        
        ## Save the image as a file (e.g., JPEG)
        #with open('captured_image.jpg', 'wb') as f:
        #    f.write(image_data.tobytes())
        img.Release()

def camera_close(camera):
    # Release the image and stop grabbing 
    camera.StopGrabbing()
    # Close the camera
    camera.Close()


if __name__ == "__main__":
    camera = init_camera()
    img = capture_frame(camera)
    save_image(img)
    camera_close(camera)
