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

#ifndef I2C_HELPER_H
#define I2C_HELPER_H

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>

extern peak_camera_handle hCam;

peak_status openCamera(void);

peak_bool checkForSuccess(peak_status status);
void signalHandler(int signal);
int cleanExit(void);

peak_bool scanInteger(uint32_t* i, uint32_t defaultValue);
peak_bool scanFileName(char* buffer, size_t buffer_size);
void waitForEnter(void);

char* modeToString(peak_i2c_mode mode);
char* registerAddressLengthToString(peak_i2c_register_address_length length);
char* registerAddressEndiannessToString(peak_endianness endianness);
char* operationStatusToString(peak_i2c_operation_status status);

#endif // I2C_HELPER_H
