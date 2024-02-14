/*!
 * \file    backend.c
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

#define VERSION "1.1.0"

#include "backend.h"
#include <malloc.h>
#include <string.h>


static peak_camera_handle hCam = PEAK_INVALID_HANDLE;
static bool isColorConversion = false;
static errorCallback g_callback = NULL;
static int g_status = 0;
static void* g_context = NULL;
static bool acquisitionRunning = false;

static bool checkForSuccess(peak_status status, bool continueExecution);


int backend_start(void)
{
    // Initialize library
    peak_status status = peak_Library_Init();
    if (!checkForSuccess(status, false))
        return status;

    // Update camera list
    status = peak_CameraList_Update(NULL);
    if (!checkForSuccess(status, false))
        return status;

    // Open first available camera
    status = peak_Camera_OpenFirstAvailable(&hCam);
    if (!checkForSuccess(status, false))
        return status;

    // Reset the camera setting to default to apply the raw pixelformat of the camera
    status = peak_Camera_ResetToDefaultSettings(hCam);
    checkForSuccess(status, true);

    return status;
}

peak_access_status backend_camera_accessStatus(void)
{
    peak_camera_id camID = peak_Camera_ID_FromHandle(hCam);
    peak_CameraList_Update(NULL);

    return peak_Camera_GetAccessStatus(camID);
}

int backend_camera_modelName(char* value)
{
    peak_camera_id camID = peak_Camera_ID_FromHandle(hCam);

    peak_camera_descriptor camDesc = { 0 };

    peak_status status = peak_Camera_GetDescriptor(camID, &camDesc);
    if (!checkForSuccess(status, true))
        return status;

    strcpy(value, camDesc.modelName);

    return status;
}

int backend_camera_serialNumber(char* value)
{
    peak_camera_id camID = peak_Camera_ID_FromHandle(hCam);

    peak_camera_descriptor camDesc = { 0 };

    peak_status status = peak_Camera_GetDescriptor(camID, &camDesc);
    if (!checkForSuccess(status, true))
        return status;

    strcpy(value, camDesc.serialNumber);

    return status;
}


int backend_exit(void)
{
    // Clean up before exit

    peak_status status = PEAK_STATUS_SUCCESS;

    // Stop acquisition, if running
    if (peak_Acquisition_IsStarted(hCam))
    {
        backend_acquisition_stop();
    }

    // Close camera, if open
    if (hCam != PEAK_INVALID_HANDLE)
    {
        // Close Camera
        status = peak_Camera_Close(hCam);
        checkForSuccess(status, true);
    }

    // Exit library
    status = peak_Library_Exit();
    checkForSuccess(status, true);

    return status;
}

int backend_acquisition_waitForBuffer(int timeout, peak_frame_handle* frame, peak_buffer* buffer)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    if (isColorConversion)
    {
        peak_frame_handle camera_frame = PEAK_INVALID_HANDLE;

        do
        {
            status = peak_Acquisition_WaitForFrame(hCam, timeout, &camera_frame);
        } while (status == PEAK_STATUS_TIMEOUT);

        if (status != PEAK_STATUS_SUCCESS)
        {
            if (acquisitionRunning)
            {
                // check if camera is still connected
                if (backend_camera_accessStatus() == PEAK_ACCESS_INVALID)
                {
                    if (g_callback)
                    {
                        g_callback("Camera was removed...", -1, g_context);
                    }
                    return status;
                }
            }
            else if (status == PEAK_STATUS_ABORTED)
            {
                // Ignore aborted exception due to acquisition stop
                return status;
            }
        }

        if (!checkForSuccess(status, true))
            return status;

        // Drop frame if it is not complete
        if (!peak_Frame_IsComplete(camera_frame))
        {
            (void)peak_Frame_Release(hCam, camera_frame);
            return PEAK_STATUS_ERROR;
        }

        // Process the received frame with the IPL to apply the previously set RGB8 PixelFormat
        status = peak_IPL_ProcessFrame(hCam, camera_frame, frame);
        checkForSuccess(status, true);

        status = peak_Frame_Buffer_Get(*frame, buffer);
        checkForSuccess(status, true);

        status = peak_Frame_Release(hCam, camera_frame);
        checkForSuccess(status, true);
    }
    else
    {
        do
        {
            status = peak_Acquisition_WaitForFrame(hCam, timeout, frame);
        } while (status == PEAK_STATUS_TIMEOUT);

        if (status != PEAK_STATUS_SUCCESS)
        {
            if (acquisitionRunning)
            {
                // check if camera is still connected
                if (backend_camera_accessStatus() == PEAK_ACCESS_INVALID)
                {
                    if (g_callback)
                    {
                        g_callback("Camera was removed...", -1, g_context);
                    }
                    return status;
                }
            }
            else if (status == PEAK_STATUS_ABORTED)
            {
                // Ignore aborted exception due to acquisition stop
                return status;
            }
        }

        if (!checkForSuccess(status, true))
            return status;

        // Drop frame if it is not complete
        if (!peak_Frame_IsComplete(*frame))
        {
            (void)peak_Frame_Release(hCam, *frame);
            return PEAK_STATUS_ERROR;
        }

        status = peak_Frame_Buffer_Get(*frame, buffer);
        checkForSuccess(status, true);
    }

    return status;
}


int backend_releaseFrame(peak_frame_handle* frame)
{
    peak_status status = peak_Frame_Release(hCam, *frame);
    checkForSuccess(status, true);

    return status;
}

int backend_acquisition_prepare(void)
{
    if (hCam == PEAK_INVALID_HANDLE)
    {
        if (g_callback)
        {
            g_callback("Trying to configure camera before it was opened. Open camera first.", -1, g_context);
        }
        return -1;
    }
    if (peak_Acquisition_IsStarted(hCam))
    {
        if (g_callback)
        {
            g_callback("Acquisition already running.", -1, g_context);
        }
        return -1;
    }

    ////////////////////////////////
    // Set IPL PixelFormat to RGB8
    ////////////////////////////////

    // Get current PixelFormat from the camera which should be the raw PixelFormat after the reset to default
    peak_pixel_format currentPixelFormat = PEAK_PIXEL_FORMAT_INVALID;
    peak_status status = peak_PixelFormat_Get(hCam, &currentPixelFormat);
    if (!checkForSuccess(status, true))
        return status;

    // No conversion needed when pixelformat is monochrom
    if (currentPixelFormat != PEAK_PIXEL_FORMAT_MONO8)
    {
        isColorConversion = true;

        bool isMono = false;
        // Check if it is a mono format
        if ((currentPixelFormat == PEAK_PIXEL_FORMAT_MONO10)
            || (currentPixelFormat == PEAK_PIXEL_FORMAT_MONO10P)
            || (currentPixelFormat == PEAK_PIXEL_FORMAT_MONO10G40_IDS)
            || (currentPixelFormat == PEAK_PIXEL_FORMAT_MONO12)
            || (currentPixelFormat == PEAK_PIXEL_FORMAT_MONO12P)
            || (currentPixelFormat == PEAK_PIXEL_FORMAT_MONO12G24_IDS))
        {
            isMono = true;
        }

        // Get list of all currently available pixelformats

        // First step: get the number of list items
        size_t pixelFormatCount = 0;
        status = peak_IPL_PixelFormat_GetList(hCam, currentPixelFormat, NULL, &pixelFormatCount);
        if (!checkForSuccess(status, true))
            return status;

        // Second step: get the list
        peak_pixel_format* pixelFormats = (peak_pixel_format*)calloc(
            pixelFormatCount, sizeof(peak_pixel_format));
        status = peak_IPL_PixelFormat_GetList(hCam, currentPixelFormat, pixelFormats, &pixelFormatCount);
        if (!checkForSuccess(status, true))
            return status;

        // Check which pixelFormat is supported
        bool mono8Supported = false;
        bool bgra8Supported = false;
        bool rgb8Supported = false;
        bool rgba8Supported = false;
        bool bgr8Supported = false;
        for (size_t i = 0; i < pixelFormatCount; i++)
        {
            if (isMono)
            {
                if (pixelFormats[i] == PEAK_PIXEL_FORMAT_MONO8)
                {
                    mono8Supported = true;
                    break;
                }
            }
            else
            {
                if (pixelFormats[i] == PEAK_PIXEL_FORMAT_BGRA8)
                {
                    // Preferred pixel format found, break
                    bgra8Supported = true;
                    break;
                }
                else if (pixelFormats[i] == PEAK_PIXEL_FORMAT_RGB8)
                {
                    rgb8Supported = true;
                }
                else if (pixelFormats[i] == PEAK_PIXEL_FORMAT_RGBA8)
                {
                    rgba8Supported = true;
                }
                else if (pixelFormats[i] == PEAK_PIXEL_FORMAT_BGR8)
                {
                    bgr8Supported = true;
                }
            }
        }

        // Set IPL PixelFormat for later host image processing
        if (mono8Supported)
        {
            status = peak_IPL_PixelFormat_Set(hCam, PEAK_PIXEL_FORMAT_MONO8);
            if (!checkForSuccess(status, true))
                return status;
        }
        else if (bgra8Supported)
        {
            status = peak_IPL_PixelFormat_Set(hCam, PEAK_PIXEL_FORMAT_BGRA8);
            if (!checkForSuccess(status, true))
                return status;
        }
        else if (rgb8Supported)
        {
            status = peak_IPL_PixelFormat_Set(hCam, PEAK_PIXEL_FORMAT_RGB8);
            if (!checkForSuccess(status, true))
                return status;
        }
        else if (rgba8Supported)
        {
            status = peak_IPL_PixelFormat_Set(hCam, PEAK_PIXEL_FORMAT_RGBA8);
            if (!checkForSuccess(status, true))
                return status;
        }
        else if (bgr8Supported)
        {
            status = peak_IPL_PixelFormat_Set(hCam, PEAK_PIXEL_FORMAT_BGR8);
            if (!checkForSuccess(status, true))
                return status;
        }
        else
        {
            // Standard pixel formats not supported, use the first available in the list
            status = peak_IPL_PixelFormat_Set(hCam, pixelFormats[0]);
            if (!checkForSuccess(status, true))
                return status;
        }

        free(pixelFormats);
    }

    // initial software trigger, if available
    peak_trigger_mode mode = { PEAK_TRIGGER_TARGET_FRAME_START, PEAK_IO_CHANNEL_SOFTWARE };

    if (PEAK_IS_WRITEABLE(peak_Trigger_Mode_GetAccessStatus(hCam, mode)))
    {
        status = backend_triggerChannel_set(0);
        if (!checkForSuccess(status, true))
            return status;
    }

    return PEAK_STATUS_SUCCESS;
}


int backend_acquisition_start(void)
{
    if (hCam == PEAK_INVALID_HANDLE)
    {
        if (g_callback)
        {
            g_callback(
                "Trying to start image acquisition on camera before it was opened. Open camera first.", -1, g_context);
        }
        return -1;
    }
    if (peak_Acquisition_IsStarted(hCam))
    {
        if (g_callback)
        {
            g_callback("Acquisition already running.", -1, g_context);
        }
        return -1;
    }

    // Start acquisition with infinite number of frames,
    // acquisition has to be explicitely stopped
    peak_status status = peak_Acquisition_Start(hCam, PEAK_INFINITE);
    checkForSuccess(status, true);
    acquisitionRunning = (status == PEAK_STATUS_SUCCESS);

    return status;
}


int backend_acquisition_stop(void)
{
    if (hCam == PEAK_INVALID_HANDLE)
    {
        if (g_callback)
        {
            g_callback(
                "Trying to stop image acquisition on camera before it was opened. Open camera first.", -1, g_context);
        }
        return -1;
    }

    if (!peak_Acquisition_IsStarted(hCam))
    {
        if (g_callback)
        {
            g_callback("No acquisition running.", -1, g_context);
        }
        return 0;
    }

    acquisitionRunning = false;

    // check if camera is still connected
    peak_access_status accesStatus = backend_camera_accessStatus();
    if (accesStatus == PEAK_ACCESS_INVALID)
    {
        // camera has already been removed, skip acquisition stop
        return PEAK_STATUS_WARNING;
    }

    // Stop acquisition
    peak_status status = peak_Acquisition_Stop(hCam);
    checkForSuccess(status, true);

    return status;
}


bool backend_acquisition_isStarted(void)
{
    return peak_Acquisition_IsStarted(hCam);
}


int backend_prepare_freerun(void)
{
    peak_status status = peak_Trigger_Enable(hCam, PEAK_FALSE);
    checkForSuccess(status, true);

    return status;
}


bool backend_trigger_isExecutable(void)
{
    return peak_Trigger_IsExecutable(hCam);
}


int backend_trigger_execute(void)
{
    if (!peak_Trigger_IsExecutable(hCam))
    {
        if (g_callback)
        {
            g_callback("Trying to trigger without trigger configuration. Configure trigger first.", -1, g_context);
        }
        return -1;
    }

    peak_status status = peak_Trigger_Execute(hCam);
    checkForSuccess(status, true);

    return status;
}

bool backend_trigger_isEnabled(void)
{
    return peak_Trigger_IsEnabled(hCam);
}


bool backend_triggerDelay_isReadable(void)
{
    return PEAK_IS_READABLE(peak_Trigger_Delay_GetAccessStatus(hCam));
}


bool backend_triggerDelay_isWriteable(void)
{
    return PEAK_IS_WRITEABLE(peak_Trigger_Delay_GetAccessStatus(hCam));
}


struct range_double backend_triggerDelay_range(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    struct range_double range = { .0, .0, .0 };
    status = peak_Trigger_Delay_GetRange(hCam, &range.min, &range.max, &range.inc);
    checkForSuccess(status, true);

    return range;
}


peak_pixel_format backend_pixelFormat_get(void)
{
    peak_pixel_format currentPixelFormat = PEAK_PIXEL_FORMAT_INVALID;

    // Get current PixelFormat...
    if (isColorConversion)
    {
        // ...from the IPL
        peak_status status = peak_IPL_PixelFormat_Get(hCam, &currentPixelFormat);
        checkForSuccess(status, true);
    }
    else
    {
        // ...from the camera
        peak_status status = peak_PixelFormat_Get(hCam, &currentPixelFormat);
        checkForSuccess(status, true);
    }

    return currentPixelFormat;
}


bool backend_exposureTime_isReadable(void)
{
    return PEAK_IS_READABLE(peak_ExposureTime_GetAccessStatus(hCam));
}


bool backend_exposureTime_isWriteable(void)
{
    return PEAK_IS_WRITEABLE(peak_ExposureTime_GetAccessStatus(hCam));
}


struct range_double backend_exposureTime_range(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    struct range_double range = { .0, .0, .0 };
    status = peak_ExposureTime_GetRange(hCam, &range.min, &range.max, &range.inc);
    checkForSuccess(status, true);

    return range;
}


double backend_exposureTime_get(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    double value = .0;
    status = peak_ExposureTime_Get(hCam, &value);
    checkForSuccess(status, true);

    return value;
}


int backend_exposureTime_set(double value)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    status = peak_ExposureTime_Set(hCam, value);
    checkForSuccess(status, true);

    return status;
}


bool backend_frameRate_isReadable(void)
{
    return PEAK_IS_READABLE(peak_FrameRate_GetAccessStatus(hCam));
}


bool backend_frameRate_isWriteable(void)
{
    return PEAK_IS_WRITEABLE(peak_FrameRate_GetAccessStatus(hCam));
}


struct range_double backend_frameRate_range(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    struct range_double range = { .0, .0, .0 };
    status = peak_FrameRate_GetRange(hCam, &range.min, &range.max, &range.inc);
    checkForSuccess(status, true);

    return range;
}


double backend_frameRate_get(void)
{
    double value = .0;
    peak_status status = peak_FrameRate_Get(hCam, &value);
    checkForSuccess(status, true);

    return value;
}


int backend_frameRate_set(double value)
{
    peak_status status = peak_FrameRate_Set(hCam, value);
    checkForSuccess(status, true);

    return status;
}


peak_roi backend_roi_get(void)
{
    peak_roi roi = { 0, 0, 0, 0 };

    peak_status status = peak_ROI_Get(hCam, &roi);
    checkForSuccess(status, true);

    return roi;
}


double backend_triggerDelay_get(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    double value = -1.0;
    status = peak_Trigger_Delay_Get(hCam, &value);
    checkForSuccess(status, true);

    return value;
}


int backend_triggerDelay_set(double value)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    status = peak_Trigger_Delay_Set(hCam, value);
    checkForSuccess(status, true);

    return status;
}


bool backend_triggerDivider_isReadable(void)
{
    return PEAK_IS_READABLE(peak_Trigger_Divider_GetAccessStatus(hCam));
}


bool backend_triggerDivider_isWriteable(void)
{
    return PEAK_IS_WRITEABLE(peak_Trigger_Divider_GetAccessStatus(hCam));
}


struct range_int backend_triggerDivider_range(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    struct range_int range = { 0, 0, 0 };
    status = peak_Trigger_Divider_GetRange(hCam, &range.min, &range.max, &range.inc);
    checkForSuccess(status, true);

    return range;
}


uint32_t backend_triggerDivider_get(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    uint32_t value = 0;
    status = peak_Trigger_Divider_Get(hCam, &value);
    checkForSuccess(status, true);

    return value;
}


int backend_triggerDivider_set(uint32_t value)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    status = peak_Trigger_Divider_Set(hCam, value);
    checkForSuccess(status, true);

    return status;
}


bool backend_triggerChannel_isReadable(void)
{
    return PEAK_IS_READABLE(peak_Trigger_GetAccessStatus(hCam));
}


bool backend_triggerChannel_isWriteable(void)
{
    return PEAK_IS_WRITEABLE(peak_Trigger_GetAccessStatus(hCam));
}


int backend_triggerChannel_get(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    peak_io_channel channel = PEAK_IO_CHANNEL_INVALID;
    peak_trigger_mode mode = { PEAK_TRIGGER_TARGET_INVALID, PEAK_IO_CHANNEL_INVALID };
    status = peak_Trigger_Mode_Get(hCam, &mode);
    checkForSuccess(status, true);

    channel = mode.ioChannel;

    switch (channel)
    {
    case PEAK_IO_CHANNEL_INVALID:
        return -1;
    case PEAK_IO_CHANNEL_SOFTWARE:
        return 0;
    case PEAK_IO_CHANNEL_TRIGGER_INPUT:
        return 1;
    case PEAK_IO_CHANNEL_GPIO_1:
        return 2;
    case PEAK_IO_CHANNEL_GPIO_2:
        return 3;
    default:
        break;
    }

    return -2;
}


int backend_triggerChannel_set(int value)
{
    peak_io_channel channel = PEAK_IO_CHANNEL_INVALID;
    peak_trigger_mode mode = { PEAK_TRIGGER_TARGET_FRAME_START, PEAK_IO_CHANNEL_INVALID };

    switch (value)
    {
    case 0:
        mode.ioChannel = PEAK_IO_CHANNEL_SOFTWARE;
        break;
    case 1:
        mode.ioChannel = PEAK_IO_CHANNEL_TRIGGER_INPUT;
        break;
    case 2:
        mode.ioChannel = PEAK_IO_CHANNEL_GPIO_1;
        break;
    case 3:
        mode.ioChannel = PEAK_IO_CHANNEL_GPIO_2;
        break;
    default:
        return 1;
    }

    peak_status status = PEAK_STATUS_SUCCESS;

    status = peak_Trigger_Mode_Set(hCam, mode);
    checkForSuccess(status, true);
    if (status != PEAK_STATUS_SUCCESS)
    {
        // selected io channel is not available
        return status;
    }

    status = peak_Trigger_Enable(hCam, PEAK_TRUE);
    return status;
}


struct channel_available backend_triggerChannel_getAvailable(void)
{
    struct channel_available result = { false, false, false, false };

    peak_status status = PEAK_STATUS_SUCCESS;

    peak_io_channel channel = PEAK_IO_CHANNEL_INVALID;
    peak_trigger_mode mode = { PEAK_TRIGGER_TARGET_FRAME_START, PEAK_IO_CHANNEL_INVALID };

    // check availability of software trigger
    mode.ioChannel = PEAK_IO_CHANNEL_SOFTWARE;
    result.software = PEAK_IS_WRITEABLE(peak_Trigger_Mode_GetAccessStatus(hCam, mode));

    // check availability of hardware trigger on trigger input
    mode.ioChannel = PEAK_IO_CHANNEL_TRIGGER_INPUT;
    result.hardware = PEAK_IS_WRITEABLE(peak_Trigger_Mode_GetAccessStatus(hCam, mode));

    // check availability of hardware trigger on GPIO1
    mode.ioChannel = PEAK_IO_CHANNEL_GPIO_1;
    result.gpio1 = PEAK_IS_WRITEABLE(peak_Trigger_Mode_GetAccessStatus(hCam, mode));

    // check availability of hardware trigger on GPIO2
    mode.ioChannel = PEAK_IO_CHANNEL_GPIO_2;
    result.gpio2 = PEAK_IS_WRITEABLE(peak_Trigger_Mode_GetAccessStatus(hCam, mode));

    return result;
}


bool backend_triggerEdge_isReadable(void)
{
    return PEAK_IS_READABLE(peak_Trigger_Edge_GetAccessStatus(hCam));
}


bool backend_triggerEdge_isWriteable(void)
{
    return PEAK_IS_WRITEABLE(peak_Trigger_Edge_GetAccessStatus(hCam));
}


int backend_triggerEdge_get(void)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    peak_trigger_edge edge = PEAK_TRIGGER_EDGE_INVALID;
    status = peak_Trigger_Edge_Get(hCam, &edge);
    checkForSuccess(status, true);

    switch (edge)
    {
    case PEAK_TRIGGER_EDGE_INVALID:
        return -1;
    case PEAK_TRIGGER_EDGE_RISING:
        return 0;
    case PEAK_TRIGGER_EDGE_FALLING:
        return 1;
    case PEAK_TRIGGER_EDGE_ANY:
        return 2;
    }

    return -2;
}


int backend_triggerEdge_set(int value)
{
    peak_trigger_edge edge = PEAK_TRIGGER_EDGE_INVALID;

    switch (value)
    {
    case -1:
        edge = PEAK_TRIGGER_EDGE_INVALID;
        return 1;
    case 0:
        edge = PEAK_TRIGGER_EDGE_RISING;
        break;
    case 1:
        edge = PEAK_TRIGGER_EDGE_FALLING;
        break;
    case 2:
        edge = PEAK_TRIGGER_EDGE_ANY;
        break;
    default:
        return 1;
    }

    peak_status status = PEAK_STATUS_SUCCESS;

    status = peak_Trigger_Edge_Set(hCam, edge);

    return status;
}


struct edge_available backend_triggerEdge_getAvailable(void)
{
    struct edge_available result = { false, false, false };

    peak_status status = PEAK_STATUS_SUCCESS;

    // First step: get the number of list items
    size_t count = 0;
    status = peak_Trigger_Edge_GetList(hCam, NULL, &count);
    if (!checkForSuccess(status, true))
    {
        struct edge_available falseResult = { false, false, false };
        return falseResult;
    }

    // Second step: get the list
    peak_trigger_edge* edgeList = (peak_trigger_edge*)calloc(count, sizeof(peak_trigger_edge));
    status = peak_Trigger_Edge_GetList(hCam, edgeList, &count);
    if (!checkForSuccess(status, true))
    {
        struct edge_available falseResult = { false, false, false };
        return falseResult;
    }

    for (int i = 0; i < count; ++i)
    {
        switch (edgeList[i])
        {
        case PEAK_TRIGGER_EDGE_RISING:
            result.rising = true;
            break;
        case PEAK_TRIGGER_EDGE_FALLING:
            result.falling = true;
            break;
        case PEAK_TRIGGER_EDGE_ANY:
            result.any = true;
            break;
        default:
            break;
        }
    }

    free(edgeList);
    return result;
}


void backend_errorCallback_connect(void* receiver, errorCallback slot)
{
    g_context = receiver;
    g_callback = slot;
}


// Returns true, if function was successful.
// Returns false, if function returned with an error.
// If continueExecution == false, the backend is exited.
static bool checkForSuccess(peak_status status, bool continueExecution)
{
    if (PEAK_ERROR(status))
    {
        size_t lastErrorMessageSize = 0;

        // Get size of error message
        peak_Library_GetLastError(NULL, NULL, &lastErrorMessageSize);

        // Allocate and zero-initialize the char array for the error message
        char* errorMessage = (char*)calloc((lastErrorMessageSize) / sizeof(char), sizeof(char));

        // Get the error message
        peak_Library_GetLastError(&status, errorMessage, &lastErrorMessageSize);

        // Connect error handling mechanism, in this case a callback that can be used from C++ class
        if (g_callback)
        {
            g_callback(errorMessage, status, g_context);
        }
        free(errorMessage);

        if (!continueExecution)
        {
            backend_exit();
        }

        return false;
    }
    return true;
}
