/*!
* \file    helper.c
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

#include "helper.h"
#include <ids_peak_comfort_c/ids_peak_comfort_c.h>
#include <errno.h>
#include <inttypes.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
#include <Windows.h>
#endif

const char* containerTypeToString(peak_video_container container)
{
    if (container == PEAK_VIDEO_CONTAINER_AVI)
        return "AVI";
    else
        return "Invalid";
}

const char* encoderTypeToString(peak_video_encoder encoder)
{
    if (encoder == PEAK_VIDEO_ENCODER_MJPEG)
        return "MJPEG";
    else
        return "Invalid";
}

peak_status openCamera()
{
    // Initialize library
    peak_status status = peak_Library_Init();
    if (!checkForSuccess(status))
        return status;

    // Update the camera list
    status = peak_CameraList_Update(NULL);
    if (!checkForSuccess(status))
        return status;

    // Get the length of the camera list
    size_t cameraListLength = 0;
    status = peak_CameraList_Get(NULL, &cameraListLength);
    if (!checkForSuccess(status))
        return status;

    // Return if no camera was found
    if (cameraListLength == 0)
    {
        printf("No camera found. \n");
        return PEAK_STATUS_CAMERA_NOT_FOUND;
    }

    // Get the camera list
    peak_camera_descriptor* cameraList = (peak_camera_descriptor*)calloc(
        cameraListLength, sizeof(peak_camera_descriptor));

    status = peak_CameraList_Get(cameraList, &cameraListLength);
    if (!checkForSuccess(status))
    {
        free(cameraList);
        return status;
    }

    // Print all available cameras
    printf("Cameras available: \n");
    for (size_t i = 0; i < cameraListLength; ++i)
    {
        peak_camera_descriptor camera = cameraList[i];
        printf("%zu: %s (Serial: %s, ID: %"PRIu64")\n", i, camera.modelName, camera.serialNumber, camera.cameraID);
    }

    // Select a camera via user input and open it
    size_t selectedCamera = cameraListLength;
    peak_bool scanSucceeded = PEAK_FALSE;
    while (selectedCamera >= cameraListLength || !scanSucceeded)
    {
        printf("\nSelect camera to open: ");
        scanSucceeded = scanInteger(&selectedCamera);

        if (selectedCamera >= cameraListLength || !scanSucceeded)
        {
            printf("Please select a valid camera. \n");
        }
    }

    status = peak_Camera_Open(cameraList[selectedCamera].cameraID, &hCam);
    if (!checkForSuccess(status))
    {
        free(cameraList);
        return status;
    }

    // Free the camera list, not needed any longer
    free(cameraList);

    // Print information about the opened camera
    peak_camera_descriptor cameraInfo;
    status = peak_Camera_GetDescriptor(peak_Camera_ID_FromHandle(hCam), &cameraInfo);
    checkForSuccess(status);

    printf("\nOpened camera: %s (Serial: %s, ID: %"PRIu64")\n", cameraInfo.modelName, cameraInfo.serialNumber, cameraInfo.cameraID);

    return PEAK_STATUS_SUCCESS;
}

peak_status adjustFrameRateToCameraRange(double* frameRate)
{
    double minFramerate = .0;
    double maxFramerate = .0;
    double incFramerate = .0;

    peak_status status = peak_FrameRate_GetRange(hCam, &minFramerate, &maxFramerate, &incFramerate);
    if (checkForSuccess(status))
    {
        if (*frameRate < minFramerate)
        {
            *frameRate = minFramerate;
        }
        else if (*frameRate > maxFramerate)
        {
            *frameRate = maxFramerate;
        }
    }

    return status;
}

peak_status configureColorConversion(peak_bool* hasColorConversion)
{
    // Get the camera's pixel format
    peak_pixel_format pixelFormat = PEAK_PIXEL_FORMAT_INVALID;
    peak_status status = peak_PixelFormat_Get(hCam, &pixelFormat);
    if (!checkForSuccess(status))
    {
        return status;
    }

    // No conversion needed when the pixel format is 8 Bit monochrome
    if (pixelFormat == PEAK_PIXEL_FORMAT_MONO8)
    {
        *hasColorConversion = PEAK_FALSE;
        return PEAK_STATUS_SUCCESS;
    }

    *hasColorConversion = PEAK_TRUE;
    peak_bool isMono = isMonoPixelFormat(pixelFormat);

    // Get list of all available IPL conversion PixelFormats for current source PixelFormat
    size_t pixelFormatCount = 0;
    status = peak_IPL_PixelFormat_GetList(hCam, pixelFormat, NULL, &pixelFormatCount);
    if (!checkForSuccess(status))
        return status;

    peak_pixel_format* pixelFormats = (peak_pixel_format*)calloc(
        pixelFormatCount, sizeof(peak_pixel_format));
    status = peak_IPL_PixelFormat_GetList(hCam, pixelFormat, pixelFormats, &pixelFormatCount);
    if (!checkForSuccess(status))
    {
        free(pixelFormats);
        return status;
    }

    // Set IPL PixelFormat for later host image processing
    peak_pixel_format conversionFormat = PEAK_PIXEL_FORMAT_INVALID;

    for (size_t i = 0; i < pixelFormatCount; i++)
    {
        if (isMono && pixelFormats[i] == PEAK_PIXEL_FORMAT_MONO8)
        {
            conversionFormat = PEAK_PIXEL_FORMAT_MONO8;
            break;
        }
        else if (pixelFormats[i] == PEAK_PIXEL_FORMAT_BGRA8)
        {
            conversionFormat = PEAK_PIXEL_FORMAT_BGRA8;
            break;
        }
        else if (pixelFormats[i] == PEAK_PIXEL_FORMAT_RGB8)
        {
            conversionFormat = PEAK_PIXEL_FORMAT_RGB8;
            break;
        }
        else if (pixelFormats[i] == PEAK_PIXEL_FORMAT_RGBA8)
        {
            conversionFormat = PEAK_PIXEL_FORMAT_RGBA8;
            break;
        }
        else if (pixelFormats[i] == PEAK_PIXEL_FORMAT_BGR8)
        {
            conversionFormat = PEAK_PIXEL_FORMAT_BGR8;
            break;
        }
    }

    free(pixelFormats);

    if (conversionFormat == PEAK_PIXEL_FORMAT_INVALID)
    {
        printf("\nCannot convert camera pixel format into valid pixel format for video recording.\n");
        return PEAK_STATUS_ERROR;
    }

    status = peak_IPL_PixelFormat_Set(hCam, conversionFormat);
    checkForSuccess(status);
    return status;
}

peak_status acquireFrame(peak_frame_handle* frame, peak_bool hasColorConversion)
{
    if (hasColorConversion)
    {
        peak_frame_handle camera_frame = PEAK_INVALID_HANDLE;

        // Wait to receive the camera frame
        peak_status status = peak_Acquisition_WaitForFrame(hCam, 1000, &camera_frame);
        if(!PEAK_SUCCESS(status))
        {
            return status;
        }

        // Process the received frame with the IPL
        status = peak_IPL_ProcessFrame(hCam, camera_frame, frame);
        if(!PEAK_SUCCESS(status))
        {
            peak_Frame_Release(hCam, camera_frame);
            return status;
        }

        // Release the camera frame
        status = peak_Frame_Release(hCam, camera_frame);
        if(!PEAK_SUCCESS(status))
        {
            peak_Frame_Release(hCam, *frame);
            return status;
        }

        return status;
    }
    else
    {
        // Wait to receive the camera frame
        peak_status status = peak_Acquisition_WaitForFrame(hCam, 1000, frame);

        return status;
    }
}

peak_bool isMonoPixelFormat(peak_pixel_format pixelFormat)
{
    if ((pixelFormat == PEAK_PIXEL_FORMAT_MONO8)
        || (pixelFormat == PEAK_PIXEL_FORMAT_MONO10)
        || (pixelFormat == PEAK_PIXEL_FORMAT_MONO10P)
        || (pixelFormat == PEAK_PIXEL_FORMAT_MONO10G40_IDS)
        || (pixelFormat == PEAK_PIXEL_FORMAT_MONO12)
        || (pixelFormat == PEAK_PIXEL_FORMAT_MONO12P)
        || (pixelFormat == PEAK_PIXEL_FORMAT_MONO12G24_IDS))
    {
        return PEAK_TRUE;
    }
    else
    {
        return PEAK_FALSE;
    }
}

peak_bool scanFileName(char* buffer, size_t buffer_size)
{
    char buf[256];
    char* ret = fgets(buf, 256, stdin);

    if (ret == NULL)
    {
        return PEAK_FALSE;
    }

    const size_t strLen = strlen(buf);
    buf[strLen - 1] = '\0';
    return convertToUtf8(buf, strLen, buffer, buffer_size);
}

peak_bool convertToUtf8(char* inBuffer, size_t inBuffer_size, char* outBuffer, size_t outBuffer_size)
{
#ifdef WIN32
    // Make sure the filename is UTF-8 encoded for Windows
    wchar_t wstr[256];
    int charCount = MultiByteToWideChar(CP_OEMCP, 0, inBuffer, -1, wstr, (int)sizeof(wstr));
    if (charCount == 0)
        return PEAK_FALSE;

    wstr [charCount] = 0;

    int bufSize = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, outBuffer, 0, NULL, NULL);
    if(bufSize > outBuffer_size - 1)
        return PEAK_FALSE;

    charCount = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, outBuffer, bufSize, NULL, NULL);
    if (charCount == 0)
        return PEAK_FALSE;

    outBuffer [charCount] = 0;
#else
    memcpy(outBuffer, inBuffer, outBuffer_size);
#endif

    return PEAK_TRUE;
}

void signalHandler(int signal)
{
    // Break signal handler
    printf("\nAborting application.\n");

    // Clean up before exit
    peak_status status;

    // Stop acquisition, if running
    if (peak_Acquisition_IsStarted(hCam))
    {
        status = peak_Acquisition_Stop(hCam);
        checkForSuccess(status);
    }

    // Close camera, if open
    if (hCam != PEAK_INVALID_HANDLE)
    {
        status = peak_Camera_Close(hCam);
        checkForSuccess(status);
    }

    // Exit library
    status = peak_Library_Exit();
    checkForSuccess(status);

    exit(0);
}

int cleanExit()
{
    printf("\nExiting program.\n");
    waitForEnter();

    // Clean up before exit
    peak_status status;

    // Stop acquisition, if running
    if (peak_Acquisition_IsStarted(hCam))
    {
        status = peak_Acquisition_Stop(hCam);
        checkForSuccess(status);
    }

    // Close camera, if open
    if (hCam != PEAK_INVALID_HANDLE)
    {
        status = peak_Camera_Close(hCam);
        checkForSuccess(status);
    }

    // Exit library
    status = peak_Library_Exit();
    checkForSuccess(status);

    return status;
}

// Returns PEAK_TRUE, if function was successful.
// Returns PEAK_FALSE, if function returned with an error. If continueExecution == PEAK_FALSE,
// the program exits.
peak_bool checkForSuccess(peak_status checkStatus)
{
    if (PEAK_ERROR(checkStatus))
    {
        peak_status lastErrorCode = PEAK_STATUS_SUCCESS;
        size_t lastErrorMessageSize = 0;

        // Get size of error message
        peak_status status = peak_Library_GetLastError(&lastErrorCode, NULL, &lastErrorMessageSize);
        if (PEAK_ERROR(status))
        {
            // Something went wrong getting the last error!
            printf("Last-Error: Getting last error code failed! Status: %#06x\n", status);
            return PEAK_FALSE;
        }

        if (checkStatus != lastErrorCode)
        {
            // Another error occured in the meantime. Proceed with the last error.
            printf("Last-Error: Another error occured in the meantime!\n");
        }

        // Allocate and zero-initialize the char array for the error message
        char* lastErrorMessage = (char*)calloc((lastErrorMessageSize) / sizeof(char), sizeof(char));
        if (lastErrorMessage == NULL)
        {
            // Cannot allocate lastErrorMessage. Most likely not enough Memory.
            printf("Last-Error: Failed to allocate memory for the error message!\n");
            free(lastErrorMessage);
            return PEAK_FALSE;
        }

        // Get the error message
        status = peak_Library_GetLastError(&lastErrorCode, lastErrorMessage, &lastErrorMessageSize);
        if (PEAK_ERROR(status))
        {
            // Unable to get error message. This shouldn't ever happen.
            printf("Last-Error: Getting last error message failed! Status: %#06x; Last error code: %#06x\n", status,
                lastErrorCode);
            free(lastErrorMessage);
            return PEAK_FALSE;
        }

        printf("Last-Error: %s | Code: %#06x\n", lastErrorMessage, lastErrorCode);
        free(lastErrorMessage);

        return PEAK_FALSE;
    }
    return PEAK_TRUE;
}

void waitForEnter()
{
    printf("\n");
#if defined(WIN32)
    system("pause");
#elif defined(__linux__)
    printf("Press enter to exit.\n");
    system("read _");
#else
#    warning("Operating system not implemented!")
#endif
}

void scanSingleCharacter(char* str)
{
#if defined(WIN32)
    (void)scanf_s("%c", str, 1);
#elif defined(__linux__)
    (void)scanf("%c", str);
#endif
}

peak_bool scanInteger(size_t* i)
{
    char buffer [256];
    char *endptr = NULL;

    fgets (buffer, 256, stdin);
    errno = 0;
    unsigned long int value = strtoul(buffer, &endptr, 0);

    if (errno == 0 && strcmp("\n", endptr) == 0)
    {
        *i = value;
        return PEAK_TRUE;
    }

    return PEAK_FALSE;
}