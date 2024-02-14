/*!
 * \file    backend.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.0.0
 *
 * \brief   The Backend implements an easy way to display images from a
 *          camera in a QT widgets window. It can be used for other applications as well.
 *
 * \version 1.1.0
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

#ifndef BACKEND_H
#define BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>
#include <stdbool.h>


struct range_double
{
    double min;
    double max;
    double inc;
};

int backend_start(void);

int backend_acquisition_prepare(void);
int backend_acquisition_start(void);
int backend_acquisition_stop(void);

peak_access_status backend_camera_accessStatus(void);
int backend_camera_modelName(char* value);
int backend_camera_serialNumber(char* value);
int backend_exit(void);

bool backend_exposureTime_isReadable(void);
bool backend_exposureTime_isWriteable(void);
struct range_double backend_exposureTime_range(void);
double backend_exposureTime_get(void);
int backend_exposureTime_set(double value);

bool backend_frameRate_isReadable(void);
bool backend_frameRate_isWriteable(void);
struct range_double backend_frameRate_range(void);
double backend_frameRate_get(void);
int backend_frameRate_set(double value);

bool backend_mirrorLeftRight_isReadable(void);
bool backend_mirrorLeftRight_isWriteable(void);
bool backend_mirrorLeftRight_isEnabled(void);
int backend_mirrorLeftRight_enable(bool enable);

bool backend_mirrorUpDown_isReadable(void);
bool backend_mirrorUpDown_isWriteable(void);
bool backend_mirrorUpDown_isEnabled(void);
int backend_mirrorUpDown_enable(bool enable);

peak_roi backend_roi_get(void);

peak_pixel_format backend_pixelFormat_get(void);

int backend_acquisition_waitForBuffer(int timeout, peak_frame_handle*, peak_buffer*);
int backend_releaseFrame(peak_frame_handle*);

typedef void (*errorCallback)(const char*, const int, void* errorCallbackContext);
void backend_errorCallback_connect(void* receiver, errorCallback slot);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BACKEND_H
