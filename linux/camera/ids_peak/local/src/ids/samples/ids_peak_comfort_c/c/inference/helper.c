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
#include <Windows.h>
#endif

char* typeToString(peak_inference_type type)
{
    switch(type)
    {
    case PEAK_INFERENCE_TYPE_INVALID:
        return "PEAK_INFERENCE_TYPE_INVALID";
    case PEAK_INFERENCE_TYPE_DETECTION:
        return "PEAK_INFERENCE_TYPE_DETECTION";
    case PEAK_INFERENCE_TYPE_CLASSIFICATION:
        return "PEAK_INFERENCE_TYPE_CLASSIFICATION";
    }
    return "Unknown type";
}

char* modeToString(peak_inference_preprocessing_mode mode)
{
    switch(mode)
    {
    case PEAK_INFERENCE_PREPROCESSING_MODE_INVALID:
        return "peak_PREPROCESSING_MODE_INVALID";
    case PEAK_INFERENCE_PREPROCESSING_MODE_CAFFE:
        return "peak_PREPROCESSING_MODE_CAFFE";
    case PEAK_INFERENCE_PREPROCESSING_MODE_TENSORFLOW:
        return "peak_PREPROCESSING_MODE_TENSORFLOW";
    }
    return "Unknown mode";
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
        printf("%zu: %s (Serial: %s, ID: %"PRIu64")\n", i, camera.modelName, camera.serialNumber, camera.cameraID);
    }

    // Select a camera via user input and open it
    size_t selectedCamera = cameraCount;

    peak_bool scanSucceeded = PEAK_FALSE;
    while (selectedCamera >= cameraCount || !scanSucceeded)
    {
        printf("\nSelect camera to open: ");
        scanSucceeded = scanInteger((uint32_t*)&selectedCamera);

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

    printf("\nOpened camera: %s (Serial: %s, ID: %"PRIu64")\n", cameraInfo.modelName, cameraInfo.serialNumber, cameraInfo.cameraID);

    return PEAK_STATUS_SUCCESS;
}

peak_frame_handle getFrameHandle()
{
    peak_status status = peak_Acquisition_Start(hCam, 1);
    checkForSuccess(status);
    peak_frame_handle hFrame = PEAK_INVALID_HANDLE;
    status = peak_Acquisition_WaitForFrame(hCam, 5000, &hFrame);
    checkForSuccess(status);
    return hFrame;
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

peak_bool scanInteger(uint32_t* i)
{
    char buffer[256];
    char* endptr = NULL;

    char* ret = fgets(buffer, 256, stdin);
    if (ret == NULL)
    {
        return PEAK_FALSE;
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

peak_bool convertToUtf8(char* inBuffer, size_t inBuffer_size, char* outBuffer, size_t outBuffer_size)
{
#ifdef WIN32
    // Make sure the filename is UTF-8 encoded for Windows
    wchar_t wstr[256];
    int charCount = MultiByteToWideChar(CP_OEMCP, 0, inBuffer, -1, wstr, (int)sizeof(wstr));
    if (charCount == 0)
    {
        return PEAK_FALSE;
    }

    wstr [charCount] = 0;

    int bufSize = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, outBuffer, 0, NULL, NULL);
    if (bufSize > outBuffer_size - 1)
    {
        return PEAK_FALSE;
    }

    charCount = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, outBuffer, bufSize, NULL, NULL);
    if (charCount == 0)
    {
        return PEAK_FALSE;
    }

    outBuffer [charCount] = 0;
#else
    memcpy(outBuffer, inBuffer, outBuffer_size);
#endif

    return PEAK_TRUE;
}

peak_bool isPixelFormatSupported(peak_pixel_format format)
{
    peak_pixel_format unsupported_formats[] = { PEAK_PIXEL_FORMAT_BAYER_GR10G40_IDS,
        PEAK_PIXEL_FORMAT_BAYER_RG10G40_IDS, PEAK_PIXEL_FORMAT_BAYER_GB10G40_IDS,
        PEAK_PIXEL_FORMAT_BAYER_BG10G40_IDS, PEAK_PIXEL_FORMAT_BAYER_GR12G24_IDS,
        PEAK_PIXEL_FORMAT_BAYER_RG12G24_IDS, PEAK_PIXEL_FORMAT_BAYER_GB12G24_IDS,
        PEAK_PIXEL_FORMAT_BAYER_BG12G24_IDS, PEAK_PIXEL_FORMAT_MONO10G40_IDS,
        PEAK_PIXEL_FORMAT_MONO12G24_IDS, PEAK_PIXEL_FORMAT_BAYER_GR10P,
        PEAK_PIXEL_FORMAT_BAYER_GR12P, PEAK_PIXEL_FORMAT_BAYER_RG10P,
        PEAK_PIXEL_FORMAT_BAYER_RG12P, PEAK_PIXEL_FORMAT_BAYER_GB10P,
        PEAK_PIXEL_FORMAT_BAYER_GB12P, PEAK_PIXEL_FORMAT_BAYER_BG10P,
        PEAK_PIXEL_FORMAT_BAYER_BG12P, PEAK_PIXEL_FORMAT_MONO10P, PEAK_PIXEL_FORMAT_MONO12P,
        PEAK_PIXEL_FORMAT_RGB10P32, PEAK_PIXEL_FORMAT_BGR10P32 };

    const size_t numberOfUnsupportedFormats = sizeof(unsupported_formats)/sizeof(unsupported_formats[0]);

    for (size_t i = 0; i < numberOfUnsupportedFormats; i++)
    {
        if(format == unsupported_formats[i])
        {
            return PEAK_FALSE;
        }
    }

    return PEAK_TRUE;
}

peak_bool trySetSupportedPixelFormat()
{
    size_t numberOfSupportedFormats = 0;
    peak_status status = peak_PixelFormat_GetList(hCam, NULL, &numberOfSupportedFormats);
    checkForSuccess(status);
    peak_pixel_format* supp_formats = (peak_pixel_format*)calloc(numberOfSupportedFormats, sizeof(peak_pixel_format));
    status = peak_PixelFormat_GetList(hCam, supp_formats, &numberOfSupportedFormats);
    checkForSuccess(status);
    peak_pixel_format format_to_set = PEAK_PIXEL_FORMAT_INVALID;
    for (size_t i = 0; i < numberOfSupportedFormats; i++)
    {
        peak_pixel_format format = supp_formats[i];
        if (isPixelFormatSupported(format))
        {
            format_to_set = format;
            break;
        }
    }
    free(supp_formats);
    status = peak_PixelFormat_Set(hCam, format_to_set);
    checkForSuccess(status);
    if(status == PEAK_STATUS_SUCCESS)
    {
        return PEAK_TRUE;
    }
    return PEAK_FALSE;
}

void printCNNInfo(peak_inference_handle hInference)
{
    peak_inference_info info;
    peak_status status = peak_Inference_CNN_Info_Get(hInference, &info);
    checkForSuccess(status);

    peak_version cnnFileVersion = info.fileVersion;
    const char* lighthouseID = info.lighthouseID;
    uint64_t creationTime = info.creationTime_s;
    const char* projectName = info.projectName;
    const char* netName = info.networkName;
    peak_inference_type inferenceType = info.inferenceType;
    size_t inputWidth = info.inputWidth;
    size_t inputHeight = info.inputHeight;
    peak_pixel_format inputPixelFormat = info.inputPixelFormat;
    peak_inference_preprocessing_mode preprocessingMode = info.preprocessingMode;
    size_t numberOfClasses = info.numberOfClasses;

    struct tm* timeInfo = NULL;
    time_t sinceEpoch = (time_t)creationTime;
#if (__STDC_VERSION__ >= 201112L)
    localtime_s(timeInfo, &sinceEpoch);
#else
    timeInfo = localtime(&sinceEpoch);
#endif
    char timeBuf[20];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", timeInfo);

    printf("\nFile information:\n");
    printf("\tFile version: %u.%u.%u.%u\n", cnnFileVersion.major, cnnFileVersion.minor, cnnFileVersion.subMinor, cnnFileVersion.patch);
    printf("\tLighthouse ID: %s\n", lighthouseID);
    printf("\tTime of creation: %s (%" PRIu64 ")\n", timeBuf, creationTime);
    printf("\tProject name: %s\n", projectName);
    printf("\tName of net: %s\n", netName);
    printf("\tType of Inference: %s\n", typeToString(inferenceType));
    printf("\tInput width: %zu\n", inputWidth);
    printf("\tInput height: %zu\n", inputHeight);
    printf("\tInput pixel format: %d\n", inputPixelFormat);
    printf("\tPreprocessing mode: %s\n", modeToString(preprocessingMode));
    printf("\tNumber of classes: %zu\n", numberOfClasses);
}

peak_bool printInferenceStatisticsInfo(peak_inference_handle hInference, peak_inference_result_data result)
{
    peak_inference_statistics statistics;

    peak_status status = peak_Inference_Statistics_Get(hInference, &statistics);
    checkForSuccess(status);

    // Print the inference statistics
    printf("\nInference statistics:\n");
    printf("\tSuccessful inferences: %u \n", statistics.numInferencesSuccessful);
    printf("\tFailed inferences: %u \n", statistics.numInferencesFailed);
    printf("\tPreprocessing time: %u us\n", result.preprocessing_time_us);
    printf("\tInference time: %u us\n", result.inference_time_us);

    return statistics.numInferencesFailed > 0;
}

void printResultTypeInfo(void* resList, peak_inference_type type, size_t resultCount)
{
    printf("\n%zu result entries:\n", resultCount);
    for (size_t i = 0; i < resultCount; i++)
    {
        if (type == PEAK_INFERENCE_TYPE_DETECTION)
        {
            peak_inference_result_detection* resultList = (peak_inference_result_detection*)resList;
            printf("\nResult %zu of Detection: %s\n", i, resultList[i].label);
            printf("\twith score: %f\n", resultList[i].score);
            printf("\tin place (x, y, width, height): (%u, %u, %u, %u)\n", resultList[i].rect.offset.x, resultList[i].rect.offset.y, resultList[i].rect.size.width, resultList[i].rect.size.height);
        }
        else if (type == PEAK_INFERENCE_TYPE_CLASSIFICATION)
        {
            peak_inference_result_classification* resultList = (peak_inference_result_classification*)resList;
            printf("\nResult of Classification: %s\n", resultList[i].label);
            printf("\twith score of: %f\n", resultList[i].score);
        }
        else
        {
            printf("\nCurrently no information on given type.\n");
        }
    }
}

