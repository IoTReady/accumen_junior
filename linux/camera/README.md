# Usage

```
from camera import intialise_camera, capture_optimised

# Intialise camera and store the returned objects in global vars
cam, stream = initialise_camera()

# Use global vars to capture an optimised frame whenever needed
result = capture_optimised(cam, stream)
print(result) # json
```

# Notes
- install the ids_peak pip package by wheel file (/local/share/ids/bindings/python/wheel/)
- `initialise_camera()` also connects to Redis in a blocking call. This will block indefinitely if Redis is not currently running.
