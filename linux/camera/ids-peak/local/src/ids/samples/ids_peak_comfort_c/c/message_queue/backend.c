/*!
 * \file    backend.c
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-04-24
 * \since   2.5.0
 *
 * \brief   The Backend implements the api access. It can be used for other applications as well.
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

#include "backend.h"
#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

static errorCallback g_callback = NULL;
static void* g_context = NULL;

static bool checkForSuccess(peak_status status);

int backend_start(void)
{
    // Initialize library
    peak_status status = peak_Library_Init();
    checkForSuccess(status);

    return status;
}

int backend_exit(void)
{
    // Exit library
    peak_status status = peak_Library_Exit();
    checkForSuccess(status);

    return status;
}

void backend_acquisition_single_frame(peak_camera_handle hCam)
{
    peak_status status = peak_Acquisition_Stop(hCam);
    if (!checkForSuccess(status))
    {
        return;
    }

    status = peak_Acquisition_Start(hCam, 1);
    checkForSuccess(status);
}


void backend_errorCallback_connect(void* receiver, errorCallback slot)
{
    g_context = receiver;
    g_callback = slot;
}

void backend_cameralist_update(void)
{
    peak_status status = peak_CameraList_Update(NULL);
    checkForSuccess(status);
}

void backend_event_trigger_test(peak_camera_handle hCam)
{
    peak_status status = peak_GFA_EnableWriteAccess(hCam, PEAK_TRUE);
    if (!checkForSuccess(status))
    {
        return;
    }

    status = peak_GFA_Command_Execute(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TestEventGenerate");
    if (!checkForSuccess(status))
    {
        peak_GFA_EnableWriteAccess(hCam, PEAK_FALSE);
        return;
    }

    status = peak_GFA_Command_WaitForDone(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TestEventGenerate", 1000);
    if (!checkForSuccess(status))
    {
        peak_GFA_EnableWriteAccess(hCam, PEAK_FALSE);
        return;
    }

    status = peak_GFA_EnableWriteAccess(hCam, PEAK_FALSE);
    checkForSuccess(status);
}

peak_camera_handle backend_open_camera(peak_camera_id id)
{
    peak_camera_handle hCam;
    peak_status status = peak_Camera_Open(id, &hCam);
    if (!checkForSuccess(status))
    {
        return PEAK_INVALID_HANDLE;
    }

    return hCam;
}

void backend_close_camera(peak_camera_handle hCam)
{
    peak_status status = peak_Camera_Close(hCam);
    checkForSuccess(status);
}

void backend_messagequeue_message_disable(
    peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type type)
{
    peak_status status = peak_MessageQueue_DisableMessage(hMessageQueue, hCam, type);
    checkForSuccess(status);
}

void backend_messagequeue_message_enable(
    peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type type)
{
    peak_status status = peak_MessageQueue_EnableMessage(hMessageQueue, hCam, type);
    checkForSuccess(status);
}

bool backend_messagequeue_message_supported(
    peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type type)
{
    return peak_MessageQueue_IsMessageSupported(hMessageQueue, hCam, type);
}

peak_message_queue_handle backend_messagequeue_prepare(void)
{
    peak_message_queue_handle hMessageQueue;
    peak_status status = peak_MessageQueue_Create(&hMessageQueue);
    if (!checkForSuccess(status))
    {
        return PEAK_INVALID_HANDLE;
    }

    status = peak_MessageQueue_Start(hMessageQueue);
    if (!checkForSuccess(status))
    {
        return PEAK_INVALID_HANDLE;
    }

    return hMessageQueue;
}

void backend_messagequeue_cleanup(peak_message_queue_handle hMessageQueue)
{
    peak_status status = peak_MessageQueue_Stop(hMessageQueue);
    if (!checkForSuccess(status))
    {
        return;
    }

    status = peak_MessageQueue_Destroy(hMessageQueue);
    checkForSuccess(status);
}

peak_message_handle backend_messagequeue_get_message(peak_message_queue_handle hMessageQueue)
{
    peak_message_handle hMessage;
    peak_status status = peak_MessageQueue_WaitForMessage(hMessageQueue, PEAK_INFINITE, &hMessage);
    if(status == PEAK_STATUS_ABORTED)
    {
        return PEAK_INVALID_HANDLE;
    }

    if (!checkForSuccess(status))
    {
        return PEAK_INVALID_HANDLE;
    }

    return hMessage;
}

int backend_message_info(peak_message_handle hMessage, peak_message_info* info)
{
    peak_status status = peak_Message_GetInfo(hMessage, info);
    checkForSuccess(status);

    return status;
}

int backend_message_data_string(peak_message_handle hMessage, char* buffer, size_t bufferSize)
{
    peak_message_data_type dataType;
    peak_status status = peak_Message_Data_Type_Get(hMessage, &dataType);
    checkForSuccess(status);

    switch (dataType)
    {
    case PEAK_MESSAGE_DATA_TYPE_REMOTE_DEVICE: {
        peak_message_data_remote_device remoteDevice;
        status = peak_Message_Data_RemoteDevice_Get(hMessage, &remoteDevice);
        if (!checkForSuccess(status))
        {
            break;
        }

        int num = snprintf(buffer, bufferSize, "DeviceTimestamp: %" PRIu64, remoteDevice.timestamp_ns);
        if (num < 0 || num > bufferSize)
        {
            status = PEAK_STATUS_BUFFER_TOO_SMALL;
        }

        break;
    }

    case PEAK_MESSAGE_DATA_TYPE_REMOTE_DEVICE_FRAME: {
        peak_message_data_remote_device_frame remoteDeviceFrame;
        status = peak_Message_Data_RemoteDeviceFrame_Get(hMessage, &remoteDeviceFrame);
        if (!checkForSuccess(status))
        {
            break;
        }

        int num = snprintf(buffer, bufferSize, "DeviceTimestamp: %" PRIu64 ", FrameID: %" PRId64,
            remoteDeviceFrame.timestamp_ns, remoteDeviceFrame.frameId);
        if (num < 0 || num > bufferSize)
        {
            status = PEAK_STATUS_BUFFER_TOO_SMALL;
        }

        break;
    }

    case PEAK_MESSAGE_DATA_TYPE_REMOTE_DEVICE_TEMPERATURE: {
        peak_message_data_remote_device_temperature remoteDeviceTemperature;
        status = peak_Message_Data_RemoteDeviceTemperature_Get(hMessage, &remoteDeviceTemperature);
        if (!checkForSuccess(status))
        {
            break;
        }

        double fahrenheit = remoteDeviceTemperature.temperature * 9.0 / 5.0 + 32.0;

        int num = snprintf(buffer, bufferSize, "DeviceTimestamp: %" PRIu64 ", Temperature: %.2f °C / %.2f °F",
            remoteDeviceTemperature.timestamp_ns, remoteDeviceTemperature.temperature, fahrenheit);
        if (num < 0 || num > bufferSize)
        {
            status = PEAK_STATUS_BUFFER_TOO_SMALL;
        }

        break;
    }

    case PEAK_MESSAGE_DATA_TYPE_REMOTE_DEVICE_DROPPED: {
        peak_message_data_remote_device_dropped remoteDeviceDropped;
        status = peak_Message_Data_RemoteDeviceDropped_Get(hMessage, &remoteDeviceDropped);
        if (!checkForSuccess(status))
        {
            break;
        }

        int num = snprintf(buffer, bufferSize, "DeviceTimestamp: %" PRIu64 ", Dropped: %" PRId64,
            remoteDeviceDropped.timestamp_ns, remoteDeviceDropped.count);
        if (num < 0 || num > bufferSize)
        {
            status = PEAK_STATUS_BUFFER_TOO_SMALL;
        }

        break;
    }

    case PEAK_MESSAGE_DATA_TYPE_REMOTE_DEVICE_ERROR: {
        peak_message_data_remote_device_error remoteDeviceError;
        status = peak_Message_Data_RemoteDeviceError_Get(hMessage, &remoteDeviceError);
        if (!checkForSuccess(status))
        {
            break;
        }

        int num = snprintf(buffer, bufferSize, "DeviceTimestamp: %" PRIu64 ", ErrorType: %" PRId64,
            remoteDeviceError.timestamp_ns, remoteDeviceError.error_type);
        if (num < 0 || num > bufferSize)
        {
            status = PEAK_STATUS_BUFFER_TOO_SMALL;
        }

        break;
    }

    case PEAK_MESSAGE_DATA_TYPE_AUTOFOCUS_DATA: {
        peak_message_data_autofocus autofocusData;
        status = peak_Message_Data_AutoFocusData_Get(hMessage, &autofocusData);
        if (!checkForSuccess(status))
        {
            break;
        }

        int num = snprintf(buffer, bufferSize, "FocusValue: %d, SharpnessValue: %d", autofocusData.focusValue,
            autofocusData.sharpnessValue);
        if (num < 0 || num > bufferSize)
        {
            status = PEAK_STATUS_BUFFER_TOO_SMALL;
        }

        break;
    }

    case PEAK_MESSAGE_DATA_TYPE_INVALID:
    case PEAK_MESSAGE_DATA_TYPE_NO_DATA:
        break;
    }

    return status;
}

int backend_message_release(peak_message_handle hMessage)
{
    peak_status status = peak_Message_Release(hMessage);
    checkForSuccess(status);

    return status;
}

peak_camera_id backend_camera_id_from_handle(peak_camera_handle hCam)
{
    return peak_Camera_ID_FromHandle(hCam);
}

void backend_message_queue_stop(peak_message_queue_handle hMessageQueue)
{
    peak_status status = peak_MessageQueue_Stop(hMessageQueue);
    checkForSuccess(status);
}

void backend_message_queue_start(peak_message_queue_handle hMessageQueue)
{
    peak_status status = peak_MessageQueue_Start(hMessageQueue);
    checkForSuccess(status);
}

// Returns true, if function was successful.
// Returns false, if function returned with an error.
// If continueExecution == false, the backend is exited.
static bool checkForSuccess(peak_status status)
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

        return false;
    }
    return true;
}
