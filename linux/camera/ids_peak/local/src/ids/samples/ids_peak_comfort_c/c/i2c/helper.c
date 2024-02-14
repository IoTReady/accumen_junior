/*!
 * \file    helper.c
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-02-13
 * \since   2.4.0
 *
 * \version 1.0.0
 *
 * Copyright (C) 2023, IDS Imaging Development Systems GmbH.
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
#include <time.h>

#if defined(WIN32)
#    include <Windows.h>
#endif

char* modeToString(peak_i2c_mode mode)
{
    switch (mode)
    {
    case PEAK_I2C_MODE_INVALID:
        return "PEAK_I2C_MODE_INVALID";
    case PEAK_I2C_MODE_STANDARD:
        return "PEAK_I2C_MODE_STANDARD";
    case PEAK_I2C_MODE_FAST:
        return "PEAK_I2C_MODE_FAST";
    case PEAK_I2C_MODE_FAST_PLUS:
        return "PEAK_I2C_MODE_FAST_PLUS";
    }

    return "Unknown mode";
}

char* registerAddressLengthToString(peak_i2c_register_address_length length)
{
    switch (length)
    {
    case PEAK_I2C_REGISTER_ADDRESS_LENGTH_INVALID:
        return "PEAK_I2C_REGISTER_ADDRESS_LENGTH_INVALID";
    case PEAK_I2C_REGISTER_ADDRESS_LENGTH_0BIT:
        return "PEAK_I2C_REGISTER_ADDRESS_LENGTH_0BIT";
    case PEAK_I2C_REGISTER_ADDRESS_LENGTH_8BIT:
        return "PEAK_I2C_REGISTER_ADDRESS_LENGTH_8BIT";
    case PEAK_I2C_REGISTER_ADDRESS_LENGTH_16BIT:
        return "PEAK_I2C_REGISTER_ADDRESS_LENGTH_16BIT";
    case PEAK_I2C_REGISTER_ADDRESS_LENGTH_24BIT:
        return "PEAK_I2C_REGISTER_ADDRESS_LENGTH_24BIT";
    }

    return "Unknown register address length";
}

char* registerAddressEndiannessToString(peak_endianness endianness)
{
    switch (endianness)
    {
    case PEAK_ENDIANNESS_INVALID:
        return "PEAK_ENDIANNESS_INVALID";
    case PEAK_ENDIANNESS_BIG_ENDIAN:
        return "PEAK_ENDIANNESS_BIG_ENDIAN";
    case PEAK_ENDIANNESS_LITTLE_ENDIAN:
        return "PEAK_ENDIANNESS_LITTLE_ENDIAN";
    }

    return "Unknown register address endianness";
}

char* operationStatusToString(peak_i2c_operation_status status)
{
    switch (status)
    {
    case PEAK_I2C_OPERATION_STATUS_INVALID:
        return "PEAK_I2C_OPERATION_STATUS_INVALID";
    case PEAK_I2C_OPERATION_STATUS_READY:
        return "PEAK_I2C_OPERATION_STATUS_READY";
    case PEAK_I2C_OPERATION_STATUS_ERROR:
        return "PEAK_I2C_OPERATION_STATUS_ERROR";
    case PEAK_I2C_OPERATION_STATUS_TIMEOUT_ERROR:
        return "PEAK_I2C_OPERATION_STATUS_TIMEOUT_ERROR";
    }

    return "Unknown operation status";
}

peak_status openCamera()
{
    // Initialize library
    peak_status status = peak_Library_Init();
    if (!checkForSuccess(status))
    {
        return status;
    }

    // Update the camera list
    status = peak_CameraList_Update(NULL);
    if (!checkForSuccess(status))
    {
        return status;
    }

    // Get the number of cameras in the camera list
    size_t cameraCount = 0;
    status = peak_CameraList_Get(NULL, &cameraCount);
    if (!checkForSuccess(status))
    {
        return status;
    }

    // Return if no camera was found
    if (cameraCount == 0)
    {
        printf("No camera found. \n");
        return PEAK_STATUS_CAMERA_NOT_FOUND;
    }

    // Get the camera list
    peak_camera_descriptor* cameraList = (peak_camera_descriptor*)calloc(
        cameraCount, sizeof(peak_camera_descriptor));

    status = peak_CameraList_Get(cameraList, &cameraCount);
    if (!checkForSuccess(status))
    {
        free(cameraList);
        return status;
    }

    // Print all available cameras
    printf("Cameras available: \n");
    for (size_t i = 0; i < cameraCount; ++i)
    {
        peak_camera_descriptor camera = cameraList[i];
        printf("%zu: %s (Serial: %s, ID: %" PRIu64 ")\n", i, camera.modelName, camera.serialNumber, camera.cameraID);
    }

    // Select a camera via user input and open it
    size_t selectedCamera = cameraCount;

    peak_bool scanSucceeded = PEAK_FALSE;
    while (selectedCamera >= cameraCount || !scanSucceeded)
    {
        printf("\nSelect camera to open: ");
        scanSucceeded = scanInteger((uint32_t*)&selectedCamera, 0);

        if (selectedCamera >= cameraCount || !scanSucceeded)
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

    printf("\nOpened camera: %s (Serial: %s, ID: %" PRIu64 ")\n", cameraInfo.modelName, cameraInfo.serialNumber,
        cameraInfo.cameraID);

    return PEAK_STATUS_SUCCESS;
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

    status = peak_Camera_ResetToDefaultSettings(hCam);
    checkForSuccess(status);

    // Close camera, if open
    status = peak_Camera_Close(hCam);
    checkForSuccess(status);

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

    status = peak_Camera_ResetToDefaultSettings(hCam);
    checkForSuccess(status);

    // Close camera, if open
    status = peak_Camera_Close(hCam);
    checkForSuccess(status);

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

peak_bool scanInteger(uint32_t* i, uint32_t defaultValue)
{
    char buffer[256];
    char* endptr = NULL;

    char* ret = fgets(buffer, 256, stdin);
    if (ret == NULL)
    {
        return PEAK_FALSE;
    }

    if (0 == strcmp("\n", &buffer[0]))
    {
        *i = defaultValue;
        return PEAK_TRUE;
    }

    errno = 0;
    unsigned long int value = strtoul(buffer, &endptr, 0);

    if (errno == 0 && strcmp("\n", endptr) == 0)
    {
        *i = value;
        return PEAK_TRUE;
    }

    return PEAK_FALSE;
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

    memcpy(buffer, buf, buffer_size);

    return PEAK_TRUE;
}
