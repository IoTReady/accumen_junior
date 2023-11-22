from pypylon import pylon

def init_camera():
    try:
        # Initialize the pylon runtime system (optional)
        # pylon.pylon_initialize()
        
        # Create an instant camera object for the first available camera
        camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
        
        # Open the camera
        camera.Open()
        
        # Set exposure time in microseconds
        camera.ExposureTime.SetValue(10000)  # 10,000 microseconds (10 ms)
        
        # Set gain value in dB
        camera.Gain.SetValue(6.0)  # Set gain to 6 dB
        
        return camera
    except Exception as e:
        print(f"Error initializing camera: {e}")
        return None

def capture_frame(camera):
    try:
        # Grab one image
        camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly)
        img = camera.RetrieveResult(10000, pylon.TimeoutHandling_ThrowException)
        return img
    except Exception as e:
        print(f"Error capturing frame: {e}")
        return None

def save_image(img):
    try:
        if img is not None and img.GrabSucceeded():
            # Access the image data
            image_data = img.GetArray()
            
            # Save the image as a file (e.g., PNG)
            with open('captured_image.png', 'wb') as f:
                f.write(image_data.tobytes())
            
            # Release the image
            img.Release()
    except Exception as e:
        print(f"Error saving image: {e}")

def camera_close(camera):
    try:
        # Release the image and stop grabbing
        if camera.IsGrabbing():
            camera.StopGrabbing()
        
        # Close the camera
        camera.Close()
    except Exception as e:
        print(f"Error closing camera: {e}")

if __name__ == "__main__":
    camera = init_camera()
    if camera:
        img = capture_frame(camera)
        save_image(img)
        camera_close(camera)

