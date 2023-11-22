/*!
 * \file    backend.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.1.0
 *
 * \brief   The Backend implements an easy way to display images from a
 *          camera in a QT widgets window. It can be used for other applications as well.
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

int backend_start(void);

int backend_acquisition_prepare(void);
int backend_acquisition_start(void);
int backend_acquisition_stop(void);

bool backend_camera_isMono(void);

peak_access_status backend_camera_accessStatus(void);
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

peak_roi backend_roi_get(void);

peak_pixel_format backend_pixelFormat_get(void);

int backend_acquisition_waitForBuffer(int timeout, peak_frame_handle*, peak_buffer*);
int backend_releaseFrame(peak_frame_handle*);

int backend_ipl_brightness_exposureMode_get(void);
int backend_ipl_brightness_exposureMode_set(int value);

int backend_ipl_brightness_gainMode_get(void);
int backend_ipl_brightness_gainMode_set(int value);

int backend_ipl_brightness_roiMode_get(void);
int backend_ipl_brightness_roiMode_set(int value);

struct range_int backend_ipl_brightness_roiOffsetX_range(void);
struct range_int backend_ipl_brightness_roiOffsetY_range(void);
struct range_int backend_ipl_brightness_roiWidth_range(void);
struct range_int backend_ipl_brightness_roiHeight_range(void);
peak_roi backend_ipl_brightness_roi_get(void);
int backend_ipl_brightness_roi_set(peak_roi roi);

struct range_int backend_ipl_brightness_target_range(void);
uint32_t backend_ipl_brightness_target_get(void);
int backend_ipl_brightness_target_set(uint32_t value);

struct range_double backend_ipl_brightness_targetPercentile_range(void);
double backend_ipl_brightness_targetPercentile_get(void);
int backend_ipl_brightness_targetPercentile_set(double value);

struct range_int backend_ipl_brightness_targetTolerance_range(void);
uint32_t backend_ipl_brightness_targetTolerance_get(void);
int backend_ipl_brightness_targetTolerance_set(uint32_t value);

int backend_ipl_whiteBalance_mode_get(void);
int backend_ipl_whiteBalance_mode_set(int value);

int backend_ipl_whiteBalance_roiMode_get(void);
int backend_ipl_whiteBalance_roiMode_set(int value);

struct range_int backend_ipl_whiteBalance_roiOffsetX_range(void);
struct range_int backend_ipl_whiteBalance_roiOffsetY_range(void);
struct range_int backend_ipl_whiteBalance_roiWidth_range(void);
struct range_int backend_ipl_whiteBalance_roiHeight_range(void);
peak_roi backend_ipl_whiteBalance_roi_get(void);
int backend_ipl_whiteBalance_roi_set(peak_roi roi);

struct range_double backend_ipl_gainMaster_range(void);
double backend_ipl_gainMaster_get(void);
int backend_ipl_gainMaster_set(double value);

struct range_double backend_ipl_gainRed_range(void);
double backend_ipl_gainRed_get(void);
int backend_ipl_gainRed_set(double value);

struct range_double backend_ipl_gainGreen_range(void);
double backend_ipl_gainGreen_get(void);
int backend_ipl_gainGreen_set(double value);

struct range_double backend_ipl_gainBlue_range(void);
double backend_ipl_gainBlue_get(void);
int backend_ipl_gainBlue_set(double value);

struct range_double backend_ipl_gamma_range(void);
double backend_ipl_gamma_get(void);
int backend_ipl_gamma_set(double value);

int backend_ipl_colorCorrection_enable(bool enable);
bool backend_ipl_colorCorrection_isEnabled(void);

peak_matrix backend_ipl_colorCorrectionMatrix_get(void);
int backend_ipl_colorCorrectionMatrix_set(peak_matrix ccm);
int backend_ipl_colorCorrectionMatrix_setDefault(void);
int backend_ipl_colorCorrectionMatrix_setIdentity(void);

bool backend_ipl_mirrorLeftRight_isEnabled(void);
int backend_ipl_mirrorLeftRight_enable(bool enable);

bool backend_ipl_mirrorUpDown_isEnabled(void);
int backend_ipl_mirrorUpDown_enable(bool enable);

bool backend_ipl_hotpixelCorrection_isEnabled(void);
int backend_ipl_hotpixelCorrection_enable(bool enable);

int backend_ipl_hotpixelCorrection_resetList(void);

int backend_ipl_hotpixelCorrection_sensitivity_get(void);
int backend_ipl_hotpixelCorrection_sensitivity_set(int sensitivity);

size_t backend_ipl_hotpixelCorrection_list_getCount(void);

bool backend_ipl_edgeEnhancement_isEnabled(void);
void backend_ipl_edgeEnhancement_setEnabled(bool enabled);

uint32_t backend_ipl_edgeEnhancement_factor_get(void);
void backend_ipl_edgeEnhancement_factor_set(uint32_t);
void backend_ipl_edgeEnhancement_factor_getRange(uint32_t* min, uint32_t* max, uint32_t* inc);

peak_frame_info* backend_frameInfo_get(void);

typedef void (*errorCallback)(const char*, const int, void* errorCallbackContext);
void backend_errorCallback_connect(void* receiver, errorCallback slot);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BACKEND_H
