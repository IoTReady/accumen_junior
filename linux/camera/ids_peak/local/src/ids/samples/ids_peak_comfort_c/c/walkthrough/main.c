/*!
 * \file    main.c
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-02-01
 * \since   2.0.0
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

#define VERSION "1.0.0"

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>

void printLastError();
void waitForEnter();

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    peak_status status = PEAK_STATUS_SUCCESS;

    //////////////////////////////////////
    //          Init Library
    //////////////////////////////////////

    printf("Initialising Library...\n");
    status = peak_Library_Init();
    if(PEAK_ERROR(status))
    {
        fprintf(stderr, "ERROR: Initialising the library failed! Status: %#06x\n", status);
        printLastError();
        waitForEnter();
        return status;
    }

    //////////////////////////////////////
    //        Get camera list
    //////////////////////////////////////
    printf("Updating camera list...\n");
    status = peak_CameraList_Update(NULL);
    if(PEAK_ERROR(status))
    {
        fprintf(stderr, "ERROR: Updating the camera list failed! Status: %#06x\n", status);
        printLastError();
        (void)peak_Library_Exit();
        waitForEnter();
        return status;
    }

    printf("Getting length of camera list...\n");
    size_t cameraListLength = 0;
    status = peak_CameraList_Get(NULL, &cameraListLength);
    if(PEAK_ERROR(status))
    {
        fprintf(stderr, "ERROR: Getting the camera list length failed! Status: %#06x\n", status);
        printLastError();
        (void)peak_Library_Exit();
        waitForEnter();
        return status;
    }
    printf("Number of connected cameras: %zu\n", cameraListLength);

    // Check that at least one camera is connected
    if (cameraListLength == 0)
    {
        printf("No cameras connected!\n");
        (void)peak_Library_Exit();
        waitForEnter();
        return 0;
    }

    printf("Allocating camera list...\n");
    peak_camera_descriptor* cameraList = (peak_camera_descriptor*)malloc(
        cameraListLength * sizeof(peak_camera_descriptor));
    if (cameraList == NULL)
    {
        // Camera list cannot be allocated. Most likely the memory is full.
        fprintf(stderr, "ERROR: Failed to allocate camera list!\n");
        free(cameraList);
        (void)peak_Library_Exit();
        waitForEnter();
        return PEAK_STATUS_ERROR;
    }

    printf("Getting camera list...\n");
    status = peak_CameraList_Get(cameraList, &cameraListLength);
    if(PEAK_ERROR(status))
    {
        fprintf(stderr, "ERROR: Getting the camera list failed! Status: %#06x\n", status);
        printLastError();
        (void)peak_Library_Exit();
        free(cameraList);
        waitForEnter();
        return status;
    }

    //////////////////////////////////////
    //        List cameras
    //////////////////////////////////////
    printf("\n\n Found cameras:\n");
    printf("-------------------\n");
    printf("ID \t Modelname \t\t serialNr\n");

    for (size_t i = 0; i < cameraListLength; i++)
    {
        printf("%-8" PRIu64 " %-23s %-30s\n", cameraList[i].cameraID, cameraList[i].modelName,
               cameraList[i].serialNumber);
    }
    printf("\n\n");

    // Check that the camera list is not empty
    if (cameraListLength == 0)
    {
        printf("No camera found.\n");
        free(cameraList);
        (void)peak_Library_Exit();
        waitForEnter();
        return status;
    }


    //////////////////////////////////////
    // Open first available camera in list
    //////////////////////////////////////

    printf("Opening first accessible camera in list...\n");

    peak_camera_handle hCam = NULL;
    size_t openedIndex = 0;
    for(size_t i = 0; i < cameraListLength; i++)
    {
        if(peak_Camera_GetAccessStatus(cameraList[i].cameraID) == PEAK_ACCESS_READWRITE)
        {

            status = peak_Camera_Open(cameraList[i].cameraID, &hCam);
            if (PEAK_ERROR(status))
            {
                fprintf(stderr, "ERROR: Opening camera failed! Status: %#06x\n", status);
                printLastError();
            }
            else
            {
                openedIndex = i;
                break;
            }
        }
    }

    // Check that opening a camera was successful
    if(!hCam)
    {
        printf("Could not open any connected camera.\n");
        free(cameraList);
        (void)peak_Library_Exit();
        waitForEnter();
        return status;
    }

    peak_camera_descriptor cam = cameraList[openedIndex];
    fprintf(stderr, "Camera %" PRIu64 " opened: %s (%s)\n", cam.cameraID, cam.modelName, cam.serialNumber);

    // No more need to keep the camera list. Free it.
    free(cameraList);

    //////////////////////////////////////
    //    Reset camera to default
    //////////////////////////////////////
    
    status = peak_Camera_ResetToDefaultSettings(hCam);
    if (PEAK_ERROR(status))
    {
        fprintf(stderr, "ERROR: Resetting camera parameters failed! Status: %#06x\n", status);
        printLastError();
        (void)peak_Library_Exit();
        (void)peak_Camera_Close(hCam);
        waitForEnter();
        return status;
    }

    //////////////////////////////////////
    //    Setup framerate
    //////////////////////////////////////
    double currentFramerate = 0.0;  // Used in Acquisition to calculate timeout

    if(peak_FrameRate_GetAccessStatus(hCam) == PEAK_ACCESS_READWRITE) {
        printf("Setting framerate to its maximum...\n");
        double minFramerate = 0.0;
        double maxFramerate = 0.0;
        double incFramerate = 0.0;
        status = peak_FrameRate_GetRange(hCam, &minFramerate, &maxFramerate, &incFramerate);
        if (PEAK_ERROR(status)) {
            fprintf(stderr, "ERROR: Getting framerate range failed! Status: %#06x.\n", status);
            printLastError();
            (void) peak_Camera_Close(hCam);
            (void) peak_Library_Exit();
            waitForEnter();
            return status;
        }

        status = peak_FrameRate_Set(hCam, maxFramerate);
        if (PEAK_ERROR(status)) {
            fprintf(stderr, "ERROR: Setting framerate failed! Status: %#06x.\n", status);
            printLastError();
            (void) peak_Camera_Close(hCam);
            (void) peak_Library_Exit();
            waitForEnter();
            return status;
        }

        currentFramerate = maxFramerate;
    }
    else if(PEAK_IS_READABLE(peak_FrameRate_GetAccessStatus(hCam)))
    {
        status = peak_FrameRate_Get(hCam, &currentFramerate);
        if(PEAK_ERROR(status))
        {
            fprintf(stderr, "ERROR: Getting framerate failed! Status: %#06x.\n", status);
            printLastError();
            (void) peak_Camera_Close(hCam);
            (void) peak_Library_Exit();
            waitForEnter();
            return status;
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Framerate cannot be get or set on this camera.\n");
        (void)peak_Camera_Close(hCam);
        (void)peak_Library_Exit();
        waitForEnter();
        return PEAK_STATUS_ACCESS_DENIED;
    }

    //////////////////////////////////////
    //    Start acquisition
    //////////////////////////////////////
    unsigned int framesToAcquire = 100;

    // NOTE: Acquisition will stop automatically after framesToAcquire-Images have been received
    //       via peak_Acquisition_WaitForFrame and released via peak_Frame_Release
    status = peak_Acquisition_Start(hCam, framesToAcquire);
    if(PEAK_ERROR(status))
    {
        fprintf(stderr, "ERROR: Starting acquisition failed! Status: %#06x.\n", status);
        printLastError();
        (void)peak_Camera_Close(hCam);
        (void)peak_Library_Exit();
        waitForEnter();
        return status;
    }

    //////////////////////////////////////
    //    Acquire x frames
    //////////////////////////////////////
    unsigned int pendingFrames = framesToAcquire;
    unsigned int incompleteCount = 0;
    unsigned int timeoutCount = 0;

    // Calculate timeout for WaitForFrame (being 3 frames long)
    uint32_t three_frame_times_timeout_ms = (uint32_t)((3000.0 / currentFramerate) + 0.5);

    printf("Will acquire %i frames\n", pendingFrames);
    while(pendingFrames > 0)
    {
        peak_frame_handle hFrame;
        status = peak_Acquisition_WaitForFrame(hCam, three_frame_times_timeout_ms, &hFrame);
        if(status == PEAK_STATUS_TIMEOUT)
        {
            printf("t");
            timeoutCount++;
            if (timeoutCount > 99)
            {
                fprintf(stderr, "\nWARNING: Too many timeouts. Aborting acquisition.");
                break;
            }
            else
            {
                continue;
            }
        }
        else if(status == PEAK_STATUS_ABORTED)
        {
            printf("a");
            break;
        }
        else if(PEAK_ERROR(status))
        {
            fprintf(stderr, "ERROR: WaitForFrame failed with status %#06x.\n", status);
            printLastError();
            break;
        }

        // At this point we successfully got a frame handle. We need to release it when done!

        if(peak_Frame_IsComplete(hFrame) == PEAK_FALSE)
        {
            printf("i");
            incompleteCount++;
        }
        else
        {
            // Frame successfully received.
            printf(".");
        }

        status = peak_Frame_Release(hCam, hFrame);
        if(PEAK_ERROR(status))
        {
            fprintf(stderr, "ERROR: Releasing frame failed with status %#06x.\n", status);
            printLastError();
            break;
        }
        pendingFrames--;
    }
    printf("\n");

    // NOTE: Acquisition should be stopped automatically at this point.

    //////////////////////////////////////
    //    Print statistics
    //////////////////////////////////////
    printf("Acquisition done.\n");
    printf("Timouts: %u\n", timeoutCount);
    printf("Incompletes: %u\n", incompleteCount);

    //////////////////////////////////////
    //    Close everything
    //////////////////////////////////////
    printf("Closing camera...\n");
    (void)peak_Camera_Close(hCam);
    hCam = NULL;
    printf("Camera closed.\n");

    printf("Exiting library...\n");
    (void)peak_Library_Exit();

    printf("Library exited.\n");
    waitForEnter();
    return 0;
}

void printLastError()
{
    peak_status lastErrorCode = PEAK_STATUS_SUCCESS;
    peak_status status = PEAK_STATUS_SUCCESS;
    size_t lastErrorMessageSize = 0;

    // Get last error message size
    status = peak_Library_GetLastError(&lastErrorCode, NULL, &lastErrorMessageSize);
    if (PEAK_ERROR(status))
    {
        // Something went wrong getting the last error!
        fprintf(stderr, "Last-Error: Getting last error code failed! Status: %#06x\n", status);
        return;
    }

    // Get the corresponding error message.
    char* lastErrorMessage = (char*)malloc(lastErrorMessageSize);
    if (lastErrorMessage == NULL)
    {
        // Cannot allocate lastErrorMessage. Most likely not enough Memory.
        fprintf(stderr, "Last-Error: Failed to allocate memory for the error message!\n");
        free(lastErrorMessage);
        return;
    }
    memset(lastErrorMessage, 0, lastErrorMessageSize);

    status = peak_Library_GetLastError(&lastErrorCode, lastErrorMessage, &lastErrorMessageSize);
    if (PEAK_ERROR(status))
    {
        // Unable to get error message. This shouldn't ever happen.
        fprintf(stderr, "Last-Error: Getting last error message failed! Status: %#06x; Last error code: %#06x\n",
                status, lastErrorCode);
        free(lastErrorMessage);
        return;
    }

    printf("Last-Error: %s | Code: %#06x\n", lastErrorMessage, lastErrorCode);
    free(lastErrorMessage);
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