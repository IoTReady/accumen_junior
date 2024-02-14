/*!
 * \file    backend.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-04-24
 * \since   2.5.0
 *
 * \brief   The Backend implements an easy way to display images from a
 *          camera in a QT widgets window. It can be used for other applications as well.
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

#ifndef BACKEND_H
#define BACKEND_H

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int backend_start(void);
int backend_exit(void);
void backend_cameralist_update(void);

peak_camera_id backend_camera_id_from_handle(peak_camera_handle hCam);

peak_camera_handle backend_open_camera(peak_camera_id id);
void backend_close_camera(peak_camera_handle hCam);

void backend_acquisition_single_frame(peak_camera_handle hCam);
void backend_event_trigger_test(peak_camera_handle hCam);

bool backend_messagequeue_message_supported(
    peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type type);
void backend_messagequeue_message_disable(
    peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type type);
void backend_messagequeue_message_enable(
    peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type type);
peak_message_queue_handle backend_messagequeue_prepare(void);
void backend_messagequeue_cleanup(peak_message_queue_handle hMessageQueue);
peak_message_handle backend_messagequeue_get_message(peak_message_queue_handle hMessageQueue);
void backend_message_queue_stop(peak_message_queue_handle hMessageQueue);
void backend_message_queue_start(peak_message_queue_handle hMessageQueue);

int backend_message_release(peak_message_handle hMessage);
int backend_message_info(peak_message_handle hMessage, peak_message_info* info);
int backend_message_data_string(peak_message_handle hMessage, char* buffer, size_t bufferSize);

int backend_trigger_critical_error(peak_camera_handle hCam);

typedef void (*errorCallback)(const char*, const int, void* errorCallbackContext);
void backend_errorCallback_connect(void* receiver, errorCallback slot);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BACKEND_H
