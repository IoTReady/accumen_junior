/*!
 * \file    main.c
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.2.0
 *
 * \version 1.0.0
 *
 * Copyright (C) 2022 - 2023, IDS Imaging Development Systems GmbH.
 *
 * The information in this document is subject to change without notice
 * and should not be construed as a commitment by IDS Imaging Development Systems GmbH.
 * IDS Imaging Development Systems GmbH does not assume any responsibility for any errors
 * that may appear in this document.
 *
 * This document, or source code, is provided solely as an example of how to utilize
 * IDS Imaging Development Systems GmbH software libraries in a sample application.
 * IDS Imaging Development Systems GmbH does not assume any responsibility
 * for the use or reliability of any portion of this document.
 *
 * General permission to copy or modify is hereby granted.
 */

#define VERSION "1.0.0"

#define IMAGECOUNT 100

#include "helper.h"
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>

#if defined(WIN32)
#include <Windows.h>
#endif

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>

peak_camera_handle hCam = PEAK_INVALID_HANDLE;

int main()
{
    // Handle console closing signals
#if defined(WIN32)
    signal(SIGBREAK, signalHandler);
#elif defined(__linux__)
    signal(SIGINT, signalHandler);
#endif

    // Open a camera
    peak_status status = openCamera();
    if(!PEAK_SUCCESS(status))
    {
        cleanExit();
        return status;
    }

    // Reset the camera settings to default
    status = peak_Camera_ResetToDefaultSettings(hCam);
    checkForSuccess(status);

    /////////////////////////////////////////////////////
    //    Video file configuration
    /////////////////////////////////////////////////////

    peak_video_container container = PEAK_VIDEO_CONTAINER_AVI;
    peak_video_encoder encoder = PEAK_VIDEO_ENCODER_MJPEG;
    peak_video_handle hVideo = PEAK_INVALID_HANDLE;

    // Get path and filename from user input and open the file
    char filenameUtf8[256];
    status = PEAK_STATUS_ERROR;

    while (!PEAK_SUCCESS(status))
    {
        printf("\nSelect path and filename, e.g. \"C:/temp/video.avi\" or \"/tmp/video.avi\": ");

        peak_bool scanSucceeded = scanFileName(filenameUtf8, sizeof(filenameUtf8));
        if (scanSucceeded)
        {
            status = peak_VideoWriter_Open(&hVideo, filenameUtf8, container, encoder);
        }

        if (!PEAK_SUCCESS(status))
        {
            printf(
                "Please select a valid filename (that does not already exist) in a folder that has write access. \n");
        }
    }

    // Configure camera and video frame rate to be equal
    double frameRate = 30.0;
    status = adjustFrameRateToCameraRange(&frameRate);
    checkForSuccess(status);

    status = peak_FrameRate_Set(hCam, frameRate);
    checkForSuccess(status);

    status = peak_VideoWriter_Container_Option_Set(hVideo, PEAK_VIDEO_CONTAINER_OPTION_FRAMERATE, &frameRate, sizeof(frameRate));
    checkForSuccess(status);

    // Get the quality parameter value of the video encoder (only for MJPEG)
    uint32_t quality = 0;
    status = peak_VideoWriter_Encoder_Option_Get(hVideo, PEAK_VIDEO_ENCODER_OPTION_QUALITY, &quality, sizeof(quality), NULL);
    checkForSuccess(status);

    // Print the current configuration
    printf("Configuration:\n");
    printf("\tVideo file: \"%s\"\n", filenameUtf8);
    printf("\tContainer type: %s\n", containerTypeToString(container));
    printf("\tFrame rate: %.3f fps\n", frameRate);
    printf("\tEncoder type: %s\n", encoderTypeToString(encoder));
    printf("\tQuality: %u\n", quality);

    /////////////////////////////////////////////////////
    //    Camera and IPL Configurations
    /////////////////////////////////////////////////////

    // Configure color conversion to result in a PixelFormat that is supported by the video writer
    peak_bool colorConversionEnabled = PEAK_FALSE;
    status = configureColorConversion(&colorConversionEnabled);
    if(!PEAK_SUCCESS(status))
    {
        cleanExit();
        return PEAK_STATUS_ERROR;
    }

    /////////////////////////////////////////////////////
    //    Acquisition
    /////////////////////////////////////////////////////

    // Start acquisition with 100 frames
    const uint32_t frameCount = (uint32_t)IMAGECOUNT;
    status = peak_Acquisition_Start(hCam, frameCount);
    if (!checkForSuccess(status))
    {
        cleanExit();
        return PEAK_STATUS_ERROR;
    }

    for(int i = 0; i < frameCount; ++i)
    {
        peak_frame_handle frame = PEAK_INVALID_HANDLE;

        status = acquireFrame(&frame, colorConversionEnabled);
        if(PEAK_SUCCESS(status))
        {
            status = peak_VideoWriter_AddFrame(hVideo, frame);
            if(PEAK_SUCCESS(status))
            {
                // Frame processing was successful
                printf(".");
                fflush(stdout);
            }
            else
            {
                // Video writer error
                printf("v");
                fflush(stdout);
            }

            peak_Frame_Release(hCam, frame);
        }
        else
        {
            // General acquisition or frame processing error
            printf("e");
            fflush(stdout);
        }
    }

    /////////////////////////////////////////////////////
    //    Finish
    /////////////////////////////////////////////////////

    // Wait for the video writer to finish the last frame
    status = peak_VideoWriter_WaitUntilQueueEmpty(hVideo, 1000);
    checkForSuccess(status);

    // Print the video statistics
    printf("\nVideo statistics:\n");
    peak_video_info info;
    status = peak_VideoWriter_GetInfo(hVideo, &info);
    if(checkForSuccess(status))
    {
        printf("\tFile size: %"PRIi64" Bytes\n", info.fileSize);
        printf("\tEncoded frames: %"PRIi64"\n", info.encodedFrames);
        printf("\tDropped frames: %"PRIi64"\n", info.droppedFrames);
    }
    else
    {
        printf("Could not read video info\n");
    }

    // Closing everything
    status = peak_VideoWriter_Close(hVideo);
    if(checkForSuccess(status))
    {
        printf("\nVideo file \"%s\" successfully written:\n", filenameUtf8);
    }

    return cleanExit();
}
