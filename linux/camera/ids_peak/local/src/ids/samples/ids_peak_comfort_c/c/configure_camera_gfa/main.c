/*!
 * \file    main.c
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.0.0
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

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>


peak_camera_handle hCam = PEAK_INVALID_HANDLE;


int cleanExit();
peak_bool checkForSuccess(peak_status status, peak_bool continueExecution);
void waitForEnter();
void signalHandler(int signal);



int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Handle console closing signals
#if defined(WIN32)
    signal(SIGBREAK, signalHandler);
#elif defined(__linux__)
    signal(SIGINT, signalHandler);
#endif

    peak_status status = PEAK_STATUS_SUCCESS;

    // initialize library
    status = peak_Library_Init();
    if (!checkForSuccess(status, PEAK_FALSE))
        return status;

    // update camera list
    status = peak_CameraList_Update(NULL);
    if (!checkForSuccess(status, PEAK_FALSE))
        return status;

    // get length of camera list
    size_t cameraListLength = 0;
    status = peak_CameraList_Get(NULL, &cameraListLength);
    if (!checkForSuccess(status, PEAK_FALSE))
        return status;

    // exit program if no camera was found
    if (cameraListLength == 0)
    {
        printf("No camera found. \n");
        cleanExit();
        return 0;
    }

    // allocate memory for the camera list
    peak_camera_descriptor* cameraList = (peak_camera_descriptor*)calloc(
        cameraListLength, sizeof(peak_camera_descriptor));

    // get the camera list
    status = peak_CameraList_Get(cameraList, &cameraListLength);
    if (!checkForSuccess(status, PEAK_FALSE))
    {
        free(cameraList);
        return status;
    }

    // list all available cameras
    printf("Cameras available: \n");
    for (size_t i = 0; i < cameraListLength; ++i)
    {
        peak_camera_descriptor camera = cameraList[i];
        printf("%zu: %s (Serial: %s, ID: %"PRIu64")\n", i, camera.modelName, camera.serialNumber, camera.cameraID);
    }

    // select a camera to open
    size_t selectedCamera = 0;
    // select a camera to open via user input or remove these lines to always open the first available device
    printf("\nSelect camera to open: ");
    (void)scanf("%zu", &selectedCamera);
    if (selectedCamera >= cameraListLength)
    {
        printf("Please select a valid camera. \n");
        cleanExit();
        return 0;
    }

    // open the selected camera
    status = peak_Camera_Open(cameraList[selectedCamera].cameraID, &hCam);
    if (!checkForSuccess(status, PEAK_FALSE))
    {
        // failed to open camera, free camera list and exit
        free(cameraList);
        return status;
    }

    // free the camera list, not needed any longer
    free(cameraList);

    // check, which camera was actually opened
    peak_camera_descriptor cameraInfo;
    status = peak_Camera_GetDescriptor(peak_Camera_ID_FromHandle(hCam), &cameraInfo);
    if (!checkForSuccess(status, PEAK_FALSE))
        return status;

    printf("\nOpened camera: %s (Serial: %s, ID: %"PRIu64")\n", cameraInfo.modelName, cameraInfo.serialNumber,
        cameraInfo.cameraID);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //    GFA Sample Section (generic feature access)
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    /////////////////////////////////////////////////////
    //    GFA: Start
    /////////////////////////////////////////////////////

    // The GFA WRITE access allows you, to set values using the GFA functions.
    // If you only want to get values with GFA (READ access), you could skip
    // the following function call.

    // in this mode, convenience functions of the API are locked
    status = peak_GFA_EnableWriteAccess(hCam, PEAK_TRUE);
    if (!checkForSuccess(status, PEAK_FALSE))
        return status;

    // to access features using GFA, get the corresponding module first
    // the GFA module defines the NodeMap, that you want to address
    // e.g. for the remote device NodeMap, use PEAK_GFA_MODULE_REMOTE_DEVICE


    /////////////////////////////////////////////////////
    //    GFA: Read String
    /////////////////////////////////////////////////////

    // read a string using GFA, here the firmware version
    size_t stringLength = 0;
    char* stringValue = NULL;

    // check if the feature is readable
    if (PEAK_IS_READABLE(
            peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "DeviceFirmwareVersion")))
    {
        // get the length of the firmware version string
        status = peak_GFA_String_Get(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "DeviceFirmwareVersion", NULL, &stringLength);

        // if successful, read the firmware version
        if (checkForSuccess(status, PEAK_TRUE))
        {
            // allocate memory for the firmware version
            stringValue = (char*)calloc(stringLength, sizeof(char));

            // read the firmware version
            status = peak_GFA_String_Get(
                hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "DeviceFirmwareVersion", stringValue, &stringLength);

            // if successful, print the string to the output
            if (checkForSuccess(status, PEAK_TRUE))
            {
                printf("DeviceFirmwareVersion: GFA get value ... success\n");
                printf("\tValue: %s\n", stringValue);
            }
        }
    }
    else
    {
        printf("DeviceFirmwareVersion: GFA get value ... no read access!\n");
    }

    // if not needed any longer, free the variable
    free(stringValue);


    /////////////////////////////////////////////////////
    //    GFA: Access to other NodeMaps
    /////////////////////////////////////////////////////

    // all GFA features also work on NodeMaps of:
    // System = PEAK_GFA_MODULE_SYSTEM
    // Interface = PEAK_GFA_MODULE_INTERFACE
    // Local Device = PEAK_GFA_MODULE_LOCAL_DEVICE
    // DataStream = PEAK_GFA_MODULE_DATA_STREAM

    // the following examples are a bit abbreviated

    // read the TL version from the System NodeMap
    status = peak_GFA_String_Get(hCam, PEAK_GFA_MODULE_SYSTEM, "TLVersion", NULL, &stringLength);
    if (checkForSuccess(status, PEAK_TRUE))
    {
        stringValue = (char*)calloc(stringLength, sizeof(char));
        status = peak_GFA_String_Get(hCam, PEAK_GFA_MODULE_SYSTEM, "TLVersion", stringValue, &stringLength);
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("TLVersion (System): GFA get value ... success\n");
            printf("\tValue: %s\n", stringValue);
        }
        free(stringValue);
    }

    // read the InterfaceDisplayName from the Interface NodeMap
    status = peak_GFA_String_Get(hCam, PEAK_GFA_MODULE_INTERFACE, "InterfaceDisplayName", NULL, &stringLength);
    if (checkForSuccess(status, PEAK_TRUE))
    {
        stringValue = (char*)calloc(stringLength, sizeof(char));
        status = peak_GFA_String_Get(
            hCam, PEAK_GFA_MODULE_INTERFACE, "InterfaceDisplayName", stringValue, &stringLength);
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("InterfaceDisplayName (Interface): GFA get value ... success\n");
            printf("\tValue: %s\n", stringValue);
        }
        free(stringValue);
    }

    // read the DeviceVendorName from the LocalDevice NodeMap
    status = peak_GFA_String_Get(hCam, PEAK_GFA_MODULE_LOCAL_DEVICE, "DeviceVendorName", NULL, &stringLength);
    if (checkForSuccess(status, PEAK_TRUE))
    {
        stringValue = (char*)calloc(stringLength, sizeof(char));
        status = peak_GFA_String_Get(
            hCam, PEAK_GFA_MODULE_LOCAL_DEVICE, "DeviceVendorName", stringValue, &stringLength);
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("DeviceVendorName (LocalDevice): GFA get value ... success\n");
            printf("\tValue: %s\n", stringValue);
        }
        free(stringValue);
    }

    // read the StreamID from the DataStream NodeMap
    status = peak_GFA_String_Get(hCam, PEAK_GFA_MODULE_DATA_STREAM, "StreamID", NULL, &stringLength);
    if (checkForSuccess(status, PEAK_TRUE))
    {
        stringValue = (char*)calloc(stringLength, sizeof(char));
        status = peak_GFA_String_Get(hCam, PEAK_GFA_MODULE_DATA_STREAM, "StreamID", stringValue, &stringLength);
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("StreamID (DataStream): GFA get value ... success\n");
            printf("\tValue: %s\n", stringValue);
        }
        free(stringValue);
    }


    /////////////////////////////////////////////////////
    //    GFA: Read & Write Integer
    /////////////////////////////////////////////////////

    // read an integer using GFA, here the trigger divider (only for demonstration)
    // ATTENTION: It is not recommended to access Trigger features using GFA
    //            Please use the peak_Trigger_* functions instead!

    int64_t intValue = 0;

    // check if the feature is readable
    if (PEAK_IS_READABLE(peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider")))
    {
        // read the current value
        status = peak_GFA_Integer_Get(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider", &intValue);

        // if successful, print the string to the output
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("TriggerDivider: GFA get value ... success\n");
            printf("\tCurrent value: %"PRIi64"\n", intValue);
        }
    }
    else
    {
        printf("TriggerDivider: GFA get value ... no read access!\n");
    }

    // check if the feature is writeable
    if (PEAK_IS_WRITEABLE(peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider")))
    {
        // increase value by 1
        int64_t newIntValue = intValue + 1;

        // make sure that the new value is valid (inside incremental range, inside list)
        if (peak_GFA_Integer_HasRange(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider"))
        {
            // the feature has a range of valid values...

            int64_t intValueMin = 0;
            int64_t intValueMax = 0;
            int64_t intValueInc = 0;

            status = peak_GFA_Integer_GetRange(
                hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider", &intValueMin, &intValueMax, &intValueInc);
            if (checkForSuccess(status, PEAK_TRUE))
            {
                printf("\tRange: %"PRIi64" ... %"PRIi64", increment %"PRIi64"\n", intValueMin, intValueMax, intValueInc);

                // make the new value match the range requirements
                int64_t steps = (newIntValue - intValueMin) / intValueInc;
                newIntValue = steps * intValueInc + intValueMin;

                if (newIntValue < intValueMin)
                {
                    newIntValue = intValueMin;
                }
                if (newIntValue > intValueMax)
                {
                    newIntValue = intValueMax;
                }
            }
        }
        else
        {
            // the feature has a list of valid values...

            size_t length = 0;

            // get length of value list
            status = peak_GFA_Integer_GetList(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider", NULL, &length);
            checkForSuccess(status, PEAK_TRUE);

            // allocate memory for the value list
            int64_t* intValueList = (int64_t*)calloc(length, sizeof(int64_t));

            // get the value list
            status = peak_GFA_Integer_GetList(
                hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider", intValueList, &length);
            if (checkForSuccess(status, PEAK_TRUE))
            {
                // print the list and find a valid value
                int64_t value = intValueList[0];

                printf("\tList: ");
                for (int i = 0; i < length; ++i)
                {
                    int64_t v = intValueList[i];
                    printf("%"PRIi64" ", v);
                    // remember the largest value, that is still smaller than newFloatValue
                    if (v < newIntValue)
                    {
                        value = v;
                    }
                }
                newIntValue = value;
                printf("\n");
            }
            free(intValueList);
        }

        // write the new value
        status = peak_GFA_Integer_Set(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider", newIntValue);

        // if successful, print the string to the output
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("TriggerDivider: GFA set value ... success\n");
            printf("\tWritten value: %"PRIi64"\n", newIntValue);

            // check the current value
            if (PEAK_IS_READABLE(
                    peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider")))
            {
                status = peak_GFA_Integer_Get(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "TriggerDivider", &intValue);
                if (checkForSuccess(status, PEAK_TRUE))
                {
                    printf("\tNew current value: %"PRIi64"\n", intValue);
                }
            }
        }
    }
    else
    {
        printf("TriggerDivider: GFA set value ... no write access!\n");
    }


    /////////////////////////////////////////////////////
    //    GFA: Read & Write Float
    /////////////////////////////////////////////////////

    // read an write float using GFA, here AcquisitionFrameRateTarget
    double floatValue = 0.0;

    // check if the feature is readable
    if (PEAK_IS_READABLE(
            peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget")))
    {
        // read the current value
        status = peak_GFA_Float_Get(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget", &floatValue);

        // if successful, print the string to the output
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("AcquisitionFrameRateTarget: GFA get value ... success\n");
            printf("\tCurrent value: %4.2f\n", floatValue);
        }
    }
    else
    {
        printf("AcquisitionFrameRateTarget: GFA get value ... no read access!\n");
    }

    // check if the feature is writeable
    if (PEAK_IS_WRITEABLE(
            peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget")))
    {
        // increase value by 10%
        double newFloatValue = floatValue * 1.1;

        // make sure that the new value is valid (inside incremental range, inside list)
        if (peak_GFA_Float_HasRange(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget"))
        {
            // the feature has a range of valid values...

            double floatValueMin = 0.0;
            double floatValueMax = 0.0;
            double floatValueInc = 0.0;

            status = peak_GFA_Float_GetRange(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget",
                &floatValueMin, &floatValueMax, &floatValueInc);
            if (checkForSuccess(status, PEAK_TRUE))
            {
                printf("\tRange: %4.2f ... %4.2f, increment %4.2f\n", floatValueMin, floatValueMax, floatValueInc);

                // make the new value match the range requirements
                int64_t steps = (int64_t)((newFloatValue - floatValueMin) / floatValueInc + 0.5);
                newFloatValue = (double)steps * floatValueInc + floatValueMin;

                if (newFloatValue < floatValueMin)
                {
                    newFloatValue = floatValueMin;
                }
                if (newFloatValue > floatValueMax)
                {
                    newFloatValue = floatValueMax;
                }
            }
        }
        else
        {
            // the feature has a list of valid values...

            size_t length = 0;

            // get length of value list
            status = peak_GFA_Float_GetList(
                hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget", NULL, &length);
            checkForSuccess(status, PEAK_TRUE);

            // allocate memory for the value list
            double* floatValueList = (double*)calloc(length, sizeof(double));

            // get the value list
            status = peak_GFA_Float_GetList(
                hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget", floatValueList, &length);
            if (checkForSuccess(status, PEAK_TRUE))
            {
                // print the list and find a valid value
                double value = floatValueList[0];

                printf("\tList: ");
                for (int i = 0; i < length; ++i)
                {
                    double v = floatValueList[i];
                    printf("% 4.2f ", v);
                    // remember the largest value, that is still smaller than newFloatValue
                    if (v < newFloatValue)
                    {
                        value = v;
                    }
                }
                newFloatValue = value;
                printf("\n");
            }
            free(floatValueList);
        }

        // write the new value
        status = peak_GFA_Float_Set(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget", newFloatValue);

        // if successful, print the string to the output
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("AcquisitionFrameRateTarget: GFA set value ... success\n");
            printf("\tWritten value: %4.2f\n", newFloatValue);

            // check the current value
            if (PEAK_IS_READABLE(peak_GFA_Feature_GetAccessStatus(
                    hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget")))
            {
                status = peak_GFA_Float_Get(
                    hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTarget", &floatValue);
                if (checkForSuccess(status, PEAK_TRUE))
                {
                    printf("\tNew current value: %4.2f\n", floatValue);
                }
            }
        }
    }
    else
    {
        printf("AcquisitionFrameRateTarget: GFA set value ... no write access!\n");
    }


    /////////////////////////////////////////////////////
    //    GFA: Read & Write Boolean
    /////////////////////////////////////////////////////

    // read an write boolean using GFA, here AcquisitionFrameRateTargetEnable
    peak_bool boolValue = PEAK_FALSE;

    // check if the feature is readable
    if (PEAK_IS_READABLE(
            peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTargetEnable")))
    {
        // read the current value
        status = peak_GFA_Boolean_Get(
            hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTargetEnable", &boolValue);

        // if successful, print the string to the output
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("AcquisitionFrameRateTargetEnable: GFA get value ... success\n");
            if (boolValue)
            {
                printf("\tCurrent value: TRUE\n");
            }
            else
            {
                printf("\tCurrent value: FALSE\n");
            }
        }
    }
    else
    {
        printf("AcquisitionFrameRateTargetEnable: GFA get value ... no read access!\n");
    }

    // check if the feature is writeable
    if (PEAK_IS_WRITEABLE(
            peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTargetEnable")))
    {
        // invert the current value
        peak_bool newBoolValue = !boolValue;

        // write the new value
        status = peak_GFA_Boolean_Set(
            hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTargetEnable", newBoolValue);

        // if successful, print the string to the output
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("AcquisitionFrameRateTargetEnable: GFA set value ... success\n");
            if (newBoolValue)
            {
                printf("\tWritten value: TRUE\n");
            }
            else
            {
                printf("\tWritten value: FALSE\n");
            }

            // check the current value
            if (PEAK_IS_READABLE(peak_GFA_Feature_GetAccessStatus(
                    hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTargetEnable")))
            {
                status = peak_GFA_Boolean_Get(
                    hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "AcquisitionFrameRateTargetEnable", &boolValue);
                if (checkForSuccess(status, PEAK_TRUE))
                {
                    if (boolValue)
                    {
                        printf("\tNew current value: TRUE\n");
                    }
                    else
                    {
                        printf("\tNew current value: FALSE\n");
                    }
                }
            }
        }
    }
    else
    {
        printf("AcquisitionFrameRateTargetEnable: GFA set value ... no write access!\n");
    }


    /////////////////////////////////////////////////////
    //    GFA: Enumeration Handling (Selectors)
    /////////////////////////////////////////////////////

    // handling enumerations using GFA, here UserSetSelector (only for demonstration)
    // ATTENTION: It is not recommended to access UserSets using GFA
    //            Please use the peak_CameraSettings_* functions instead!


    size_t length;
    peak_gfa_enumeration_entry currentEntry;

    // check if the feature is readable
    if (PEAK_IS_READABLE(peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector")))
    {
        // read the list of entries

        // get the length of the entry list
        status = peak_GFA_Enumeration_GetList(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector", NULL, &length);
        checkForSuccess(status, PEAK_TRUE);

        // allocate memory for the entry list
        peak_gfa_enumeration_entry* entries = (peak_gfa_enumeration_entry*)calloc(
            length, sizeof(peak_gfa_enumeration_entry));

        // get the entry list
        status = peak_GFA_Enumeration_GetList(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector", entries, &length);
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("UserSetSelector: GFA get enumeration entries ... success\n");
            printf("\tEntries: ");
            for (int i = 0; i < length; ++i)
            {
                printf("%s ", entries[i].symbolicValue);
            }
            printf("\n");
        }

        free(entries);

        // read the current entry
        status = peak_GFA_Enumeration_Get(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector", &currentEntry);
        if (checkForSuccess(status, PEAK_TRUE))
        {
            printf("UserSetSelector: GFA get current enumeration entry ... success\n");
            printf("\tCurrent entry: %s\n", currentEntry.symbolicValue);
        }
    }
    else
    {
        printf("UserSetSelector: GFA get enumeration entries ... no read access for enumeration!\n");
    }

    // check if the feature is writeable
    if (PEAK_IS_WRITEABLE(peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector")))
    {
        // get the access of the entry that shall be activated
        // use symbolic values for direct access using the entries' names
        if (PEAK_IS_WRITEABLE(peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue(
                hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector", "Default")))
        {
            status = peak_GFA_Enumeration_SetBySymbolicValue(
                hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector", "Default");
            if (checkForSuccess(status, PEAK_TRUE))
            {
                printf("UserSetSelector: GFA set enumeration entry ... success\n");
                printf("\tWritten entry: Default\n");

                peak_GFA_Enumeration_Get(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetSelector", &currentEntry);
                printf("\tNew current entry: %s\n", currentEntry.symbolicValue);
            }
        }
        else
        {
            printf("UserSetSelector > Default: GFA set enumeration entry ... no write access for entry!\n");
        }
    }
    else
    {
        printf("UserSetSelector: GFA set enumeration entry ... no write access for enumeration!\n");
    }

    /////////////////////////////////////////////////////
    //    GFA: Execute Command
    /////////////////////////////////////////////////////

    // execute a command using GFA, here UserSetLoad (only for demonstration)
    // ATTENTION: It is not recommended to access UserSets using GFA
    //            Please use the peak_CameraSettings_* functions instead!

    // check if the feature is writeable
    if (PEAK_IS_WRITEABLE(peak_GFA_Feature_GetAccessStatus(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetLoad")))
    {
        // execute the command
        status = peak_GFA_Command_Execute(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetLoad");

        // if successful, wait until command was completely executed (optional)
        if (checkForSuccess(status, PEAK_TRUE))
        {
            status = peak_GFA_Command_WaitForDone(hCam, PEAK_GFA_MODULE_REMOTE_DEVICE, "UserSetLoad", 1000);
            checkForSuccess(status, PEAK_TRUE);
        }
        printf("UserSetLoad: GFA execute ... success\n");
    }
    else
    {
        printf("UserSetLoad: GFA execute ... no write access!\n");
    }

    /////////////////////////////////////////////////////
    //    GFA: End
    /////////////////////////////////////////////////////

    // disable GFA write access
    status = peak_GFA_EnableWriteAccess(hCam, PEAK_FALSE);
    if (!checkForSuccess(status, PEAK_TRUE))
        return status;

    // Closing everything
    cleanExit();
    return 0;
}


int cleanExit()
{
    printf("\nExiting program.\n");
    waitForEnter();

    // Clean up before exit

    peak_status status = PEAK_STATUS_SUCCESS;

    // Stop acquisition, if running
    if (peak_Acquisition_IsStarted(hCam))
    {
        // Stop acquisition
        status = peak_Acquisition_Stop(hCam);
        checkForSuccess(status, PEAK_TRUE);
    }

    // Close camera, if open
    if (hCam != PEAK_INVALID_HANDLE)
    {
        // Close Camera
        status = peak_Camera_Close(hCam);
        checkForSuccess(status, PEAK_TRUE);
    }

    // Exit library
    status = peak_Library_Exit();
    checkForSuccess(status, PEAK_TRUE);

    return status;
}


void signalHandler(int signal)
{
    // break signal handler
    printf("\nAborting application.\n");

    // Clean up before exit
    peak_status status = PEAK_STATUS_SUCCESS;

    // Stop acquisition, if running
    if (peak_Acquisition_IsStarted(hCam))
    {
        // Stop acquisition
        status = peak_Acquisition_Stop(hCam);
        checkForSuccess(status, PEAK_TRUE);
    }

    // Close camera, if open
    if (hCam != PEAK_INVALID_HANDLE)
    {
        // Close Camera
        status = peak_Camera_Close(hCam);
        checkForSuccess(status, PEAK_TRUE);
    }

    // Exit library
    status = peak_Library_Exit();
    checkForSuccess(status, PEAK_TRUE);

    exit(0);
}


// Returns PEAK_TRUE, if function was successful.
// Returns PEAK_FALSE, if function returned with an error. If continueExecution == PEAK_FALSE,
// the backend is exited.
peak_bool checkForSuccess(peak_status checkStatus, peak_bool continueExecution)
{
    if (PEAK_ERROR(checkStatus))
    {
        peak_status lastErrorCode = PEAK_STATUS_SUCCESS;
        size_t lastErrorMessageSize = 0;

        // Get size of error message
        peak_status status = peak_Library_GetLastError(&lastErrorCode, NULL, &lastErrorMessageSize);
        if (PEAK_ERROR(status))
        {
            // Something went wrong getting the last error!
            printf("Last-Error: Getting last error code failed! Status: %#06x\n", status);
            return PEAK_FALSE;
        }

        if (checkStatus != lastErrorCode)
        {
            // Another error occured in the meantime. Proceed with the last error.
            printf("Last-Error: Another error occured in the meantime!\n");
        }

        // Allocate and zero-initialize the char array for the error message
        char* lastErrorMessage = (char*)calloc((lastErrorMessageSize) / sizeof(char), sizeof(char));
        if (lastErrorMessage == NULL)
        {
            // Cannot allocate lastErrorMessage. Most likely not enough Memory.
            printf("Last-Error: Failed to allocate memory for the error message!\n");
            free(lastErrorMessage);
            return PEAK_FALSE;
        }

        // Get the error message
        status = peak_Library_GetLastError(&lastErrorCode, lastErrorMessage, &lastErrorMessageSize);
        if (PEAK_ERROR(status))
        {
            // Unable to get error message. This shouldn't ever happen.
            printf("Last-Error: Getting last error message failed! Status: %#06x; Last error code: %#06x\n", status,
                lastErrorCode);
            free(lastErrorMessage);
            return PEAK_FALSE;
        }

        printf("Last-Error: %s | Code: %#06x\n", lastErrorMessage, lastErrorCode);
        free(lastErrorMessage);

        if (!continueExecution)
        {
            cleanExit();
        }

        return PEAK_FALSE;
    }
    return PEAK_TRUE;
}


void waitForEnter()
{
    printf("\n");
#if defined(WIN32)
    system("pause");
#elif defined(__linux__)
    printf("Press enter to exit.\n");
    system("read _");
#else
#    warning("Operating system not implemented!")
#endif
}
