/*!
* \file    helper.h
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

#ifndef VIDEO_RECORDING_HELPER_H
#define VIDEO_RECORDING_HELPER_H

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>

extern peak_camera_handle hCam;

const char* containerTypeToString(peak_video_container container);
const char* encoderTypeToString(peak_video_encoder encoder);

peak_status openCamera();
peak_status adjustFrameRateToCameraRange(double* frameRate);
peak_status configureColorConversion(peak_bool* hasColorConversion);
peak_status acquireFrame(peak_frame_handle* frame, peak_bool hasColorConversion);
peak_bool isMonoPixelFormat(peak_pixel_format pixelFormat);

peak_bool scanFileName(char* buffer, size_t buffer_size);
peak_bool convertToUtf8(char* inBuffer, size_t inBuffer_size, char* outBuffer, size_t outBuffer_size);

void signalHandler(int signal);
int cleanExit();
peak_bool checkForSuccess(peak_status status);

void waitForEnter();

void scanSingleCharacter(char* c);
peak_bool scanInteger(size_t* i);

#endif // VIDEO_RECORDING_HELPER_H
