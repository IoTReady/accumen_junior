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
#    include <Windows.h>
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
    //    Create I2C Instance
    /////////////////////////////////////////////////////

    peak_i2c_handle hI2C = PEAK_INVALID_HANDLE;

    status = peak_I2C_Create(hCam, &hI2C);

    if (!PEAK_SUCCESS(status))
    {
        printf("Could not create I2C instance.");
        cleanExit();
        return status;
    }

    peak_access_status accessStatus = peak_I2C_GetAccessStatus(hI2C);

    if (!PEAK_IS_SUPPORTED(accessStatus) || !PEAK_IS_WRITEABLE(accessStatus))
    {
        if (PEAK_IS_SUPPORTED(accessStatus))
        {
            printf("Access status is not supported for I2C.");
        }
        else
        {
            printf("No read write access for the I2C instance.");
        }

        status = peak_I2C_Destroy(hI2C);
        checkForSuccess(status);

        cleanExit();
        return status;
    }

    /////////////////////////////////////////////////////
    //    I2C Mode
    /////////////////////////////////////////////////////

    peak_i2c_mode currentMode = PEAK_I2C_MODE_INVALID;

    status = peak_I2C_Mode_Get(hI2C, &currentMode);
    checkForSuccess(status);

    peak_i2c_mode* modeList = NULL;
    size_t modeListSize = 0;

    status = peak_I2C_Mode_GetList(hI2C, modeList, &modeListSize);
    checkForSuccess(status);

    modeList = malloc(sizeof(peak_i2c_mode) * modeListSize);

    status = peak_I2C_Mode_GetList(hI2C, modeList, &modeListSize);
    checkForSuccess(status);

    uint32_t currentModeIndex = 0;
    for (size_t i = 0; i < modeListSize; i++)
    {
        if (currentMode == modeList[i])
        {
            currentModeIndex = (uint32_t)i;
            break;
        }
    }

    peak_bool scanSucceeded = PEAK_FALSE;

    uint32_t modeIndex = 0;
    while ((modeIndex > modeListSize) || !scanSucceeded)
    {
        printf("\nCurrent mode: %s", modeToString(currentMode));

        for (size_t i = 0; i < modeListSize; i++)
        {
            printf("\n%s - %zu", modeToString(modeList[i]), i);
        }

        printf("\nEnter the mode to set (or press ENTER to use %s): ", modeToString(currentMode));

        scanSucceeded = scanInteger(&modeIndex, currentModeIndex);

        if ((modeIndex > modeListSize) || !scanSucceeded)
        {
            printf("Please select a valid mode. \n");
        }
    }

    peak_i2c_mode mode = modeList[modeIndex];

    free(modeList);

    if (mode != currentMode)
    {
        status = peak_I2C_Mode_Set(hI2C, mode);
        checkForSuccess(status);
    }

    /////////////////////////////////////////////////////
    //    I2C Device Address
    /////////////////////////////////////////////////////

    uint32_t currentDeviceAddress = 0;
    status = peak_I2C_DeviceAddress_Get(hI2C, &currentDeviceAddress);
    checkForSuccess(status);

    uint32_t minDeviceAddress = 0;
    uint32_t maxDeviceAddress = 0;
    status = peak_I2C_DeviceAddress_GetRange(hI2C, &minDeviceAddress, &maxDeviceAddress);
    checkForSuccess(status);

    scanSucceeded = PEAK_FALSE;

    uint32_t deviceAddress = 0;
    while ((deviceAddress < minDeviceAddress) || (deviceAddress > maxDeviceAddress) || !scanSucceeded)
    {
        printf("\nCurrent device address: %u", currentDeviceAddress);

        printf("\nEnter a device address in range %u - %u (or press ENTER to use %u): ", minDeviceAddress,
            maxDeviceAddress, currentDeviceAddress);

        scanSucceeded = scanInteger(&deviceAddress, currentDeviceAddress);

        if ((deviceAddress < minDeviceAddress) || (deviceAddress > maxDeviceAddress) || !scanSucceeded)
        {
            printf("Please select a valid device address. \n");
        }
    }

    if (deviceAddress != currentDeviceAddress)
    {
        status = peak_I2C_DeviceAddress_Set(hI2C, deviceAddress);
        checkForSuccess(status);
    }

    /////////////////////////////////////////////////////
    //    I2C Register Address
    /////////////////////////////////////////////////////

    peak_i2c_register_address_length currentRegisterAddressLength =
        PEAK_I2C_REGISTER_ADDRESS_LENGTH_INVALID;
    status = peak_I2C_RegisterAddress_Length_Get(hI2C, &currentRegisterAddressLength);
    checkForSuccess(status);

    peak_i2c_register_address_length registerAddressLengthList[] = {
        PEAK_I2C_REGISTER_ADDRESS_LENGTH_0BIT, PEAK_I2C_REGISTER_ADDRESS_LENGTH_8BIT,
        PEAK_I2C_REGISTER_ADDRESS_LENGTH_16BIT, PEAK_I2C_REGISTER_ADDRESS_LENGTH_24BIT
    };

    uint32_t currentRegisterAddressLengthIndex = 0;
    for (size_t i = 0; i < sizeof(registerAddressLengthList) / sizeof(peak_i2c_register_address_length); i++)
    {
        if (currentRegisterAddressLength == registerAddressLengthList[i])
        {
            currentRegisterAddressLengthIndex = (uint32_t)i;
            break;
        }
    }

    scanSucceeded = PEAK_FALSE;

    uint32_t registerAddressLengthIndex = 0;
    while ((registerAddressLengthIndex
               > sizeof(registerAddressLengthList) / sizeof(peak_i2c_register_address_length))
        || !scanSucceeded)
    {
        printf("\nCurrent register address length: %s", registerAddressLengthToString(currentRegisterAddressLength));

        for (size_t i = 0; i < sizeof(registerAddressLengthList) / sizeof(peak_i2c_register_address_length); i++)
        {
            printf("\n%s - %zu", registerAddressLengthToString(registerAddressLengthList[i]), i);
        }

        printf("\nEnter the register address length to set (or press ENTER to use %s): ",
            registerAddressLengthToString(currentRegisterAddressLength));

        scanSucceeded = scanInteger(&registerAddressLengthIndex, currentRegisterAddressLengthIndex);

        if ((registerAddressLengthIndex
                > sizeof(registerAddressLengthList) / sizeof(peak_i2c_register_address_length))
            || !scanSucceeded)
        {
            printf("Please select a valid register address length. \n");
        }
    }

    peak_i2c_register_address_length registerAddressLength =
        registerAddressLengthList[registerAddressLengthIndex];

    /////////////////////////////////////////////////////
    //    I2C  Register Endianness
    /////////////////////////////////////////////////////

    if (registerAddressLength == PEAK_I2C_REGISTER_ADDRESS_LENGTH_16BIT
        || registerAddressLength == PEAK_I2C_REGISTER_ADDRESS_LENGTH_24BIT)
    {
        peak_endianness currentRegisterAddressEndianness = PEAK_ENDIANNESS_INVALID;
        status = peak_I2C_RegisterAddress_Endianness_Get(hI2C, &currentRegisterAddressEndianness);
        checkForSuccess(status);

        peak_endianness registerAddressEndiannessList[] = { PEAK_ENDIANNESS_BIG_ENDIAN,
            PEAK_ENDIANNESS_LITTLE_ENDIAN };

        uint32_t currentRegisterAddressEndiannessIndex = 0;
        for (size_t i = 0; i < sizeof(registerAddressEndiannessList) / sizeof(peak_endianness); i++)
        {
            if (currentRegisterAddressEndianness == registerAddressEndiannessList[i])
            {
                currentRegisterAddressEndiannessIndex = (uint32_t)i;
                break;
            }
        }

        scanSucceeded = PEAK_FALSE;

        uint32_t registerAddressEndiannessIndex = 0;
        while ((registerAddressEndiannessIndex > sizeof(registerAddressEndiannessList) / sizeof(peak_endianness))
            || !scanSucceeded)
        {
            printf("\nCurrent register address endianness: %s",
                registerAddressEndiannessToString(currentRegisterAddressEndianness));

            for (size_t i = 0; i < sizeof(registerAddressEndiannessList) / sizeof(peak_endianness); i++)
            {
                printf("\n%s - %zu", registerAddressEndiannessToString(registerAddressEndiannessList[i]), i);
            }

            printf("\nEnter the register address endianness to set (or press ENTER to use %s): ",
                registerAddressEndiannessToString(currentRegisterAddressEndianness));

            scanSucceeded = scanInteger(&registerAddressEndiannessIndex, currentRegisterAddressEndiannessIndex);

            if ((registerAddressEndiannessIndex > sizeof(registerAddressEndiannessList) / sizeof(peak_endianness))
                || !scanSucceeded)
            {
                printf("Please select a valid register address endianness. \n");
            }
        }

        status = peak_I2C_RegisterAddress_Endianness_Set(
            hI2C, registerAddressEndiannessList[registerAddressEndiannessIndex]);
        checkForSuccess(status);
    }

    /////////////////////////////////////////////////////
    //    I2C  Register Address
    /////////////////////////////////////////////////////

    uint32_t currentRegisterAddress = 0;
    uint32_t registerAddress = 0;
    if (PEAK_I2C_REGISTER_ADDRESS_LENGTH_0BIT != registerAddressLength)
    {
        status = peak_I2C_RegisterAddress_Get(hI2C, &currentRegisterAddress);
        checkForSuccess(status);

        uint32_t minRegisterAddress = 0;
        uint32_t maxRegisterAddress = 0xFFFFFFFF >> (24 - (registerAddressLengthIndex - 1) * 8);

        scanSucceeded = PEAK_FALSE;

        while ((registerAddress < minRegisterAddress) || (registerAddress > maxRegisterAddress) || !scanSucceeded)
        {
            printf("\nCurrent register address: %u", currentRegisterAddress);

            printf("\nEnter a device register in range %u - %u (or press ENTER to use %u): ", minRegisterAddress,
                maxRegisterAddress, currentRegisterAddress);

            scanSucceeded = scanInteger(&registerAddress, currentRegisterAddress);

            if ((registerAddress < minRegisterAddress) || (registerAddress > maxRegisterAddress) || !scanSucceeded)
            {
                printf("Please select a valid register address. \n");
            }
        }
    }

    if (registerAddressLength != currentRegisterAddressLength)
    {
        status = peak_I2C_RegisterAddress_Length_Set(hI2C, registerAddressLength);
        checkForSuccess(status);
    }

    if (PEAK_I2C_REGISTER_ADDRESS_LENGTH_0BIT != registerAddressLength)
    {
        if (registerAddress != currentRegisterAddress)
        {
            status = peak_I2C_RegisterAddress_Set(hI2C, registerAddress);
            checkForSuccess(status);
        }
    }

    /////////////////////////////////////////////////////
    //    I2C AckPolling
    /////////////////////////////////////////////////////

    peak_bool currentAckPollingEnabled = peak_I2C_AckPolling_IsEnabled(hI2C);

    scanSucceeded = PEAK_FALSE;

    uint32_t ackPollingEnable = 0;
    while ((ackPollingEnable > 1) || !scanSucceeded)
    {
        printf("\nCurrent ack polling enabled: %s", currentAckPollingEnabled ? "True" : "False");

        printf("\nEnter 0 to disable or 1 to enable ack polling (or press ENTER to use %s): ",
            currentAckPollingEnabled ? "True" : "False");

        scanSucceeded = scanInteger(&ackPollingEnable, currentAckPollingEnabled);

        if ((ackPollingEnable > 1) || !scanSucceeded)
        {
            printf("Please select a valid ack polling flag. \n");
        }
    }

    uint32_t currenAckPollingTimeoutMs = 0;
    uint32_t ackPollingTimeoutMs = 0;
    if ((peak_bool)ackPollingEnable)
    {
        status = peak_I2C_AckPolling_Timeout_Get(hI2C, &currenAckPollingTimeoutMs);
        checkForSuccess(status);

        uint32_t minAckPollingTimeoutMs = 0;
        uint32_t maxAckPollingTimeoutMs = 0;
        uint32_t incAckPollingTimeoutMs = 0;
        status = peak_I2C_AckPolling_Timeout_GetRange(
            hI2C, &minAckPollingTimeoutMs, &maxAckPollingTimeoutMs, &incAckPollingTimeoutMs);
        checkForSuccess(status);

        scanSucceeded = PEAK_FALSE;

        while ((ackPollingTimeoutMs < minAckPollingTimeoutMs) || (ackPollingTimeoutMs > maxAckPollingTimeoutMs)
            || !scanSucceeded)
        {
            printf("\nCurrent ack polling timeout in ms: %u", currenAckPollingTimeoutMs);

            printf(
                "\nEnter a ack polling timeout in range %u - %u, with an increment of %u (or press ENTER to use %u): ",
                minAckPollingTimeoutMs, maxAckPollingTimeoutMs, incAckPollingTimeoutMs, currenAckPollingTimeoutMs);

            scanSucceeded = scanInteger(&ackPollingTimeoutMs, currenAckPollingTimeoutMs);

            if ((ackPollingTimeoutMs < minAckPollingTimeoutMs) || (ackPollingTimeoutMs > maxAckPollingTimeoutMs)
                || !scanSucceeded)
            {
                printf("Please select a valid ack polling timeout. \n");
            }
        }
    }

    if ((peak_bool)ackPollingEnable != currentAckPollingEnabled)
    {
        status = peak_I2C_AckPolling_Enable(hI2C, (peak_bool)ackPollingEnable);
        checkForSuccess(status);
    }

    if ((peak_bool)ackPollingEnable && (ackPollingTimeoutMs != currenAckPollingTimeoutMs))
    {
        status = peak_I2C_AckPolling_Timeout_Set(hI2C, ackPollingTimeoutMs);
        checkForSuccess(status);
    }

    /////////////////////////////////////////////////////
    //    I2C Write / Read Data
    /////////////////////////////////////////////////////

    size_t maxDataSize = 0;
    status = peak_I2C_Data_MaxSize_Get(hI2C, &maxDataSize);

    // Get path and filename from user input and open the file
    FILE* fptr = NULL;

    char filename[256];
    size_t fileSize = 0;

    while (!fptr)
    {
        printf(
            "\nSelect path and filename, e.g. \"C:/temp/i2c.bin\" or \"/tmp/i2c.bin\" (max. %zu bytes will be read): ",
            maxDataSize);

        scanSucceeded = scanFileName(filename, sizeof(filename));
        if (scanSucceeded)
        {
            fptr = fopen(filename, "rb");
        }

        if (!fptr)
        {
            printf("Please select a valid filename (that does already exist) in a folder that has read access. \n");
        }
        else
        {
            fseek(fptr, 0, SEEK_END);
            fileSize = ftell(fptr);
            rewind(fptr);

            if (0 == fileSize)
            {
                printf("The selected file %s is empty! Please select a non empty file. \n", filename);
                fclose(fptr);
                fptr = NULL;
            }
        }
    }

    size_t numberOfBytesToReadWriteToI2C = fileSize > maxDataSize ? maxDataSize : fileSize;

    uint8_t* dataToWriteToI2C = malloc(sizeof(uint8_t) * numberOfBytesToReadWriteToI2C);

    fread(dataToWriteToI2C, sizeof(uint8_t), numberOfBytesToReadWriteToI2C, fptr);

    status = peak_I2C_Data_Write(hI2C, dataToWriteToI2C, numberOfBytesToReadWriteToI2C);
    checkForSuccess(status);

    peak_i2c_operation_status operationStatusWrite = PEAK_I2C_OPERATION_STATUS_INVALID;
    status = peak_I2C_OperationStatus_Get(hI2C, &operationStatusWrite);
    checkForSuccess(status);

    printf("\nWritten to I2C with operation status: %s \n\n", operationStatusToString(operationStatusWrite));
    for (size_t i = 0; i < numberOfBytesToReadWriteToI2C; ++i)
    {
        if (0 == (i % 19))
        {
            if (0 != i)
            {
                printf("\n");
            }

            printf("\t");
        }

        printf("%02hhX ", dataToWriteToI2C[i]);
    }

    printf("\n\n");

    uint8_t* dataToReadFromI2C = malloc(sizeof(uint8_t) * numberOfBytesToReadWriteToI2C);
    memset(dataToReadFromI2C, 0x00, numberOfBytesToReadWriteToI2C);

    size_t dataSizeRead = 0;
    status = peak_I2C_Data_Read(hI2C, numberOfBytesToReadWriteToI2C, dataToReadFromI2C, &dataSizeRead);
    checkForSuccess(status);

    peak_i2c_operation_status operationStatusRead = PEAK_I2C_OPERATION_STATUS_INVALID;
    status = peak_I2C_OperationStatus_Get(hI2C, &operationStatusRead);
    checkForSuccess(status);

    printf("\nRead from I2C with operation status: %s \n\n", operationStatusToString(operationStatusRead));
    for (size_t i = 0; i < numberOfBytesToReadWriteToI2C; ++i)
    {
        if (0 == (i % 19))
        {
            if (0 != i)
            {
                printf("\n");
            }

            printf("\t");
        }

        printf("%02hhX ", dataToReadFromI2C[i]);
    }

    printf("\n\n");

    if (0 != memcmp(dataToWriteToI2C, dataToReadFromI2C, sizeof(uint8_t) * numberOfBytesToReadWriteToI2C))
    {
        printf("\nSomething goes wrong!");
    }

    free(dataToReadFromI2C);
    free(dataToWriteToI2C);

    /////////////////////////////////////////////////////
    //    Destroy I2C Instance
    /////////////////////////////////////////////////////

    peak_I2C_Destroy(hI2C);

    return cleanExit();
}
