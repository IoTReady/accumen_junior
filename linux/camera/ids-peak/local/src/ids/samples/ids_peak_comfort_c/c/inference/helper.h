/*!
* \file    helper.h
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

#ifndef AI_HELPER_H
#define AI_HELPER_H

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>

extern peak_camera_handle hCam;

peak_status openCamera(void);
peak_frame_handle getFrameHandle(void);
peak_bool isPixelFormatSupported(peak_pixel_format format);
peak_bool trySetSupportedPixelFormat(void);

peak_bool checkForSuccess(peak_status status);
void signalHandler(int signal);
int cleanExit(void);

peak_bool scanFileName(char* buffer, size_t buffer_size);
peak_bool scanInteger(uint32_t* i);
peak_bool convertToUtf8(char* inBuffer, size_t inBuffer_size, char* outBuffer, size_t outBuffer_size);
void waitForEnter(void);

void printCNNInfo(peak_inference_handle hInference);
peak_bool printInferenceStatisticsInfo(peak_inference_handle hInference, peak_inference_result_data result);
void printResultTypeInfo(void* resList, peak_inference_type type, size_t resultCount);

#endif // AI_HELPER_H
