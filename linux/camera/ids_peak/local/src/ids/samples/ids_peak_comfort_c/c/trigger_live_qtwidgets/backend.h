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

struct range_int
{
    uint32_t min;
    uint32_t max;
    uint32_t inc;
};

struct edge_available
{
    bool rising;
    bool falling;
    bool any;
};

struct channel_available
{
    bool software;
    bool hardware;
    bool gpio1;
    bool gpio2;
};

int backend_start(void);

int backend_acquisition_prepare(void);
int backend_acquisition_start(void);
int backend_acquisition_stop(void);
bool backend_acquisition_isStarted(void);

int backend_prepare_freerun(void);

peak_access_status backend_camera_accessStatus(void);
int backend_camera_modelName(char* value);
int backend_camera_serialNumber(char* value);
int backend_exit(void);

bool backend_trigger_isExecutable(void);
int backend_trigger_execute(void);
bool backend_trigger_isEnabled(void);

bool backend_triggerDelay_isReadable(void);
bool backend_triggerDelay_isWriteable(void);
struct range_double backend_triggerDelay_range(void);
double backend_triggerDelay_get(void);
int backend_triggerDelay_set(double value);

bool backend_triggerDivider_isReadable(void);
bool backend_triggerDivider_isWriteable(void);
struct range_int backend_triggerDivider_range(void);
uint32_t backend_triggerDivider_get(void);
int backend_triggerDivider_set(uint32_t value);

bool backend_triggerChannel_isReadable(void);
bool backend_triggerChannel_isWriteable(void);
int backend_triggerChannel_get(void);
int backend_triggerChannel_set(int value);
struct channel_available backend_triggerChannel_getAvailable(void);

bool backend_triggerEdge_isReadable(void);
bool backend_triggerEdge_isWriteable(void);
int backend_triggerEdge_get(void);
int backend_triggerEdge_set(int value);
struct edge_available backend_triggerEdge_getAvailable(void);

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
