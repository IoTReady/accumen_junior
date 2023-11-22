/*!
 * \file    main.c
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

#define VERSION "1.0.0"

#include "helper.h"
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#include <Windows.h>
#endif

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>
#include <string.h>

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
    if (!PEAK_SUCCESS(status))
    {
        cleanExit();
        return status;
    }

    // Reset the camera settings to default
    status = peak_Camera_ResetToDefaultSettings(hCam);
    checkForSuccess(status);

    /////////////////////////////////////////////////////
    //    CNN file config
    /////////////////////////////////////////////////////

    peak_inference_handle hInference = PEAK_INVALID_HANDLE;

    char filepathUtf8[256] = {0};
    status = PEAK_STATUS_ERROR;

    while (!PEAK_SUCCESS(status))
    {
        printf("\nSelect path and filename. (*.pcla, *.pdet): ");

        peak_bool scanSucceeded = scanFileName(filepathUtf8, sizeof(filepathUtf8));
        if (scanSucceeded)
        {
            status = peak_Inference_CNN_Open(filepathUtf8, &hInference);
        }

        if (!PEAK_SUCCESS(status))
        {
            printf("Please select a valid file.");
        }
    }

    printCNNInfo(hInference);

    peak_pixel_format format;
    status = peak_PixelFormat_Get(hCam, &format);
    if (!PEAK_SUCCESS(status))
    {
        printf("Could not get current pixel format.");
    }

    if (!isPixelFormatSupported(format))
    {
        printf("The current pixel format is not supported. Trying to change to a supported one.\n");

        if (trySetSupportedPixelFormat())
        {
            printf("Successfully changed pixel format to a supported one.\n");
        }
        else
        {
            printf("Was unable to change to a supported pixel format.\n");
            cleanExit();
            return PEAK_STATUS_ERROR;
        }
    }

    /////////////////////////////////////////////////////
    //    Acquisition
    /////////////////////////////////////////////////////

    // Get single frameHandle
    peak_frame_handle hFrame = getFrameHandle();

    /////////////////////////////////////////////////////
    //    Threshold for Inference
    /////////////////////////////////////////////////////
    // Threshold min: all data. Even the ones with the lowest probability -> Threshold max: only the data with the specified probability and above.
    uint32_t iConfRange_min = 0;
    uint32_t iConfRange_max = 0;
    uint32_t iConfRange_inc = 0;
    status = peak_Inference_ConfidenceThreshold_GetRange(hInference, &iConfRange_min, &iConfRange_max, &iConfRange_inc);
    checkForSuccess(status);
    uint32_t threshold = 0;

    peak_bool scanSucceeded = PEAK_FALSE;
    while (threshold < iConfRange_min || threshold > iConfRange_max || threshold % iConfRange_inc != 0 || !scanSucceeded)
    {
        printf("\nEnter a threshold for the inference in range %u - %u, with an increment of %u: ", iConfRange_min, iConfRange_max, iConfRange_inc);
        scanSucceeded = scanInteger(&threshold);

        if (threshold < iConfRange_min || threshold > iConfRange_max || threshold % iConfRange_inc != 0 || !scanSucceeded)
        {
            printf("Please select a valid threshold. \n");
        }
    }

    status = peak_Inference_ConfidenceThreshold_Set(hInference, threshold);
    checkForSuccess(status);

    /////////////////////////////////////////////////////
    //    Run Inference
    /////////////////////////////////////////////////////

    peak_inference_result_handle hRes = PEAK_INVALID_HANDLE;
    status = peak_Inference_CNN_ProcessFrame(hInference, hFrame, &hRes);
    checkForSuccess(status);

    status = peak_Frame_Release(hCam, hFrame);
    checkForSuccess(status);

    /////////////////////////////////////////////////////
    //    Get Inference Results
    /////////////////////////////////////////////////////
    peak_inference_result_data result = { PEAK_INFERENCE_TYPE_INVALID, PEAK_INVALID_HANDLE, 0, 0 };
    status = peak_Inference_Result_Get(hRes, &result);
    checkForSuccess(status);
    peak_bool anyInferenceFailed = PEAK_FALSE;
    if (result.type == PEAK_INFERENCE_TYPE_CLASSIFICATION)
    {
        size_t resCount = 0;
        status = peak_Inference_Result_Classification_GetList(hRes, NULL, &resCount);
        checkForSuccess(status);

        peak_inference_result_classification* resList = calloc(resCount, sizeof(peak_inference_result_classification));
        status = peak_Inference_Result_Classification_GetList(hRes, resList, &resCount);
        checkForSuccess(status);

        anyInferenceFailed = printInferenceStatisticsInfo(hInference, result);
        printResultTypeInfo(resList, result.type, resCount);

        free(resList);
    }
    else if (result.type == PEAK_INFERENCE_TYPE_DETECTION)
    {
        size_t resCount = 0;
        status = peak_Inference_Result_Detection_GetList(hRes, NULL, &resCount);
        checkForSuccess(status);

        peak_inference_result_detection* resList = calloc(resCount, sizeof(peak_inference_result_detection));
        status = peak_Inference_Result_Detection_GetList(hRes, resList, &resCount);
        checkForSuccess(status);

        anyInferenceFailed = printInferenceStatisticsInfo(hInference, result);
        printResultTypeInfo(resList, result.type, resCount);

        free(resList);
    }
    else
    {
        printf("Invalid inference type.");
    }

    /////////////////////////////////////////////////////
    //    Finish
    /////////////////////////////////////////////////////
    // Closing everything
    status = peak_Inference_Result_Release(hRes);
    checkForSuccess(status);
    status = peak_Inference_CNN_Close(hInference);
    if (checkForSuccess(status) && !anyInferenceFailed)
    {
        printf("\nInference successful for \"%s\"\n", filepathUtf8);
    }
    else
    {
        printf("\nAt least one Inference failed for \"%s\"\n", filepathUtf8);
    }

    return cleanExit();
}
