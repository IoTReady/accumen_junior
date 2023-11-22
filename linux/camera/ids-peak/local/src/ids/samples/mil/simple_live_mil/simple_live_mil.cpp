/*!
 * \file    simple_live_mil.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0.0
 *
 * \brief   Simple example for grabbing images via the GenICamTL interface
 *
 *          Synopsis:  
 *          Simple live image acquisition
 *          - in continuous mode
 *          - with image improvements by setting
 *              - a nice gamma value
 *              - calculating automatically the exposure
 *                  - in continuous mode
 *              - calculating automatically the gain value
 *                  - in continuous mode
 *
 * \version 1.0.0
 *
 * Copyright (C) 2019 - 2023, IDS Imaging Development Systems GmbH.
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

#include <algorithm>
#include <mil.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
/* define a STL compatible string that maps to MIL strings. */
typedef basic_string<MIL_TEXT_CHAR, char_traits<MIL_TEXT_CHAR>, allocator<MIL_TEXT_CHAR>> mil_string;
#define MAX_CAM 32

/* List of functions */
void ResetTriggerControls(MIL_ID MilDigitizer);
void AcquireRawImages(MIL_ID MilDigitizer);
void DisplayCameraInfos(MIL_ID MilSystem, MIL_ID MilApplication);

int MosMain(void)
{
    MIL_ID MilApplication, /* Application identifier.  */
        MilSystem, /* System identifier.       */
        MilDisplay, /* Display identifier.      */
        MilDisplayRaw, /* Display RAW bayer image. */
        MilDigitizer, /* Digitizer identifier.    */
        MilImage; /* Image buffer identifier. */

    MIL_INT BoardType;

    /* Allocate defaults. */
    MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, M_NULL, M_NULL, M_NULL);

    /* Gather and show device list (with Interfaces).*/
    MsysInquire(MilSystem, M_BOARD_TYPE, &BoardType);

    if (((BoardType & M_BOARD_TYPE_MASK) != M_GIGE_VISION) && ((BoardType & M_BOARD_TYPE_MASK) != M_USB3_VISION))
    {
        MosPrintf(MIL_TEXT("This example requires a M_GIGE_VISION or M_USB3_VISION system type.\n"));
        MosPrintf(MIL_TEXT("Please change system type in milconfig.\n"));
        MappFreeDefault(MilApplication, MilSystem, M_NULL, M_NULL, M_NULL);
        MosGetch();
        return -1;
    }

    /* Display infos of connected cameras*/
    DisplayCameraInfos(MilSystem, MilApplication);

    MosPrintf(MIL_TEXT("\n\nPress <Enter> to select and open the first available device.\n\n"));
    MosGetch();

    /* Allocate defaults, open first available camera. */
    MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, &MilDigitizer, &MilImage);

    MIL_DOUBLE MinFrameRate = 0.0, MaxFrameRate = 0.0, MinGamma = 0.0, MaxGamma = 0.0, MinExposure = 0.0,
               MaxExposure = 0.0;
    MIL_TEXT_PTR ValueAsStrVal = NULL;

    MIL_DOUBLE Gamma = 1.6;

    MIL_INT64 ExposureTime = NULL;
    MIL_DOUBLE Gain;
    MIL_TEXT_CHAR* ExposureAuto = NULL;
    MIL_TEXT_CHAR* GainAuto = NULL;

    MIL_TEXT_CHAR* ValueOff = new MIL_TEXT_CHAR[4];
    ValueOff = "Off";

    /* Set feature values to default */
    ResetTriggerControls(MilDigitizer);

    /* Set pixel format to Raw8 (BayerRG8). */
    AcquireRawImages(MilDigitizer);

    /* Min and max of gamma, exposure and framerate*/
    MdigInquireFeature(MilDigitizer, M_FEATURE_MIN, MIL_TEXT("Gamma"), M_TYPE_DOUBLE, &MinGamma);
    MdigInquireFeature(MilDigitizer, M_FEATURE_MAX, MIL_TEXT("Gamma"), M_TYPE_DOUBLE, &MaxGamma);
    MdigInquireFeature(MilDigitizer, M_FEATURE_MIN, MIL_TEXT("ExposureTime"), M_TYPE_DOUBLE, &MinExposure);
    MdigInquireFeature(MilDigitizer, M_FEATURE_MAX, MIL_TEXT("ExposureTime"), M_TYPE_DOUBLE, &MaxExposure);
    MdigInquireFeature(MilDigitizer, M_FEATURE_MIN, MIL_TEXT("AcquisitionFrameRate"), M_TYPE_DOUBLE, &MinFrameRate);
    MdigInquireFeature(MilDigitizer, M_FEATURE_MAX, MIL_TEXT("AcquisitionFrameRate"), M_TYPE_DOUBLE, &MaxFrameRate);

    /*Set framerate to 25 or, if not possible, to max*/
    if (MaxFrameRate >= 25.0)
    {
        MIL_DOUBLE AcquisitionFrameRateInHz = 25.0;
        MdigControlFeature(
            MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("AcquisitionFrameRate"), M_TYPE_DOUBLE, &AcquisitionFrameRateInHz);
    }
    else
    {
        MdigControlFeature(
            MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("AcquisitionFrameRate"), M_TYPE_DOUBLE, &MaxFrameRate);
    }

    /* Grab continuously. */
    MdigGrabContinuous(MilDigitizer, MilImage);

    /* When a key is pressed, halt. */
    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("Continuous image grab in progress.\n"));

    MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));
    MosGetch();

    /* Stop continuous grab. */
    MdigHalt(MilDigitizer);

    /* Pause to show the result. */
    MosPrintf(MIL_TEXT("Continuous grab stopped.\n\n"));
    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("Simple Live with Visualization\n\n"));
    MosPrintf(MIL_TEXT("   * Image improvements with\n"));
    MosPrintf(MIL_TEXT("       * Gamma\n"));
    MosPrintf(MIL_TEXT("       * Auto Exposure\n"));
    MosPrintf(MIL_TEXT("       * Auto Gain\n"));
    MosPrintf(MIL_TEXT("       * nice colour format\n"));
    MosPrintf(MIL_TEXT(" * in continious mode\n\n"));
    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));

    MosPrintf(MIL_TEXT("Press <Enter> to do a single image grab with gamma = 1.6.\n\n"));
    MosGetch();

    MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, "Gamma", M_TYPE_DOUBLE, &Gamma);
    /* Monoshot grab. */
    MdigGrab(MilDigitizer, MilImage);

    /* Pause to show the result. */
    MosPrintf(MIL_TEXT("Gamma image.\n\n"));
    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("Press <Enter> to do a raw image.\n\n"));
    MosGetch();

    MIL_ID MilRawBayer;
    MIL_INT BayerPattern; /* Bayer pattern set in the DCF. */

    MbufAlloc2d(MilSystem, (MIL_INT)MdigInquire(MilDigitizer, M_SIZE_X, M_NULL),
        (MIL_INT)MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL), 8 + M_UNSIGNED, M_IMAGE + M_GRAB + M_PROC + M_DISP,
        &MilRawBayer);
    MbufClear(MilRawBayer, 0x0);

    MdigInquire(MilDigitizer, M_BAYER_PATTERN, &BayerPattern);
    if (BayerPattern == M_NULL)
    {
        MosPrintf(MIL_TEXT("You must have a bayer camera and a bayer enabled DCF to run this ") MIL_TEXT("example\n"));
        MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage);
        return 1;
    }

    /* Grab a raw bayer image. */
    MdigControl(MilDigitizer, M_BAYER_CONVERSION, M_DISABLE);
    MdigGrab(MilDigitizer, MilRawBayer);
    MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplayRaw);

    /* Display the source image (Bayer). */
    MdispSelect(MilDisplayRaw, MilRawBayer);

    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("Press <Enter> to show the debayered raw image.\n\n"));
    MosGetch();

    MbufBayer(MilRawBayer, MilImage, M_DEFAULT, BayerPattern);

    MbufClear(MilRawBayer, 0);

    /* Re-enable the bayer conversion. */
    MdigControl(MilDigitizer, M_BAYER_CONVERSION, M_ENABLE);
    MbufFree(MilRawBayer);
    MdispFree(MilDisplayRaw);
    auto counter = 0;

    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("\nPress <Enter> to run Exposure Continuous and Gain Continuous.\n\n"));
    MosGetch();
    MosPrintf(MIL_TEXT("Please wait for the results:\n"));

    /* Set the automatic Exposure Mode and the automatic Gain mode to continuous for one hundred frames */
    counter = 100;
    MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, "ExposureAuto", M_TYPE_STRING, "Continuous");
    MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, "GainAuto", M_TYPE_STRING, "Continuous");
    do
    {
        MdigGrab(MilDigitizer, MilImage);
        counter--;
        MosPrintf(MIL_TEXT("."));

    } while (counter > 0);

    MdigInquireFeature(MilDigitizer, M_FEATURE_VALUE, "ExposureTime", M_TYPE_INT64, &ExposureTime);
    MdigInquireFeature(MilDigitizer, M_FEATURE_VALUE, "Gain", M_TYPE_DOUBLE, &Gain);
    MosPrintf(MIL_TEXT("\nExposureTime after ExposureAuto = Continuous:  %lld"), (long long)ExposureTime);
    MosPrintf(MIL_TEXT("\nGain after ExposureAuto = Continuous:          %f\n\n"), Gain);

    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
    MosGetch();

    /* Free defaults. */
    MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage);

    return 0;
}

/* Display connected camera infos with vendor name and interface */
void DisplayCameraInfos(MIL_ID MilSystem, MIL_ID MilApplication)
{
    MIL_ID MilDigitizer;
    MIL_INT MaxCam = MAX_CAM, NbCameras = 0, VendorStLen = 0, ModelStLen;

    MIL_TEXT_PTR CamVendor;
    MIL_TEXT_PTR CamModel;
    MIL_TEXT_PTR Interface;

    MsysInquire(MilSystem, M_NUM_CAMERA_PRESENT, &NbCameras);
    if (NbCameras > MaxCam)
    {
        NbCameras = MaxCam;
    }
    MosPrintf(MIL_TEXT("SIMPLE EXAMPLE FOR GRABBING IMAGES VIA THE GENICAMTL INTERFACE \n"));
    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(
        MIL_TEXT("There are %d camera%s detected.\n"), (int)NbCameras, NbCameras > 1 ? MIL_TEXT("s") : MIL_TEXT(""));
    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("Camera infos:\n"));
    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    MosPrintf(MIL_TEXT("%-14s%-13s%9s\n"), MIL_TEXT("Vendor"), MIL_TEXT("Model"), MIL_TEXT("Interface"));


    MosPrintf(
        MIL_TEXT("----------------------------") MIL_TEXT("------------------------------------+--------------\n"));
    for (MIL_INT DeviceNum = 0; DeviceNum < NbCameras; DeviceNum++)
    {
        MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
        MdigAlloc(MilSystem, DeviceNum, MIL_TEXT("M_DEFAULT"), M_DEV_NUMBER, &MilDigitizer);
        MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);


        /* Inquire information related to the camera being allocated. */
        MdigInquire(MilDigitizer, M_CAMERA_VENDOR_SIZE, &VendorStLen);
        MdigInquire(MilDigitizer, M_CAMERA_MODEL_SIZE, &ModelStLen);

        CamVendor = new MIL_TEXT_CHAR[VendorStLen];
        CamModel = new MIL_TEXT_CHAR[ModelStLen];

        MdigInquire(MilDigitizer, M_CAMERA_VENDOR, CamVendor);
        MdigInquire(MilDigitizer, M_CAMERA_MODEL, CamModel);
        MIL_INT Len = 0, PacketSize = 0;
        MdigInquire(MilDigitizer, M_GC_PACKET_SIZE, &PacketSize);
        MdigInquire(MilDigitizer, M_GC_INTERFACE_NAME_SIZE, &Len);
        if (Len)
        {
            Interface = new MIL_TEXT_CHAR[Len];
            MdigInquire(MilDigitizer, M_GC_INTERFACE_NAME, Interface);
        }

        MosPrintf(MIL_TEXT("%-14.13s%-13.12s%9s\n"), CamVendor, CamModel, Interface);

        MdigFree(MilDigitizer);
    }

    MappFreeDefault(MilApplication, MilSystem, M_NULL, M_NULL, M_NULL);
}

/* Set pixel format to Raw8 (BayerRG8). */
void AcquireRawImages(MIL_ID MilDigitizer)
{
    MIL_INT Len = 0, PixFrmtCount = 0;
    MIL_TEXT_PTR* PixelFormats = NULL;

    MdigInquireFeature(
        MilDigitizer, M_FEATURE_ENUM_ENTRY_COUNT, MIL_TEXT("PixelFormat"), M_TYPE_MIL_INT, &PixFrmtCount);
    if (PixFrmtCount)
    {
        PixelFormats = new MIL_TEXT_PTR[PixFrmtCount];
        for (MIL_INT i = 0; i < PixFrmtCount; i++)
        {
            MdigInquireFeature(MilDigitizer, M_FEATURE_ENUM_ENTRY_NAME + M_STRING_SIZE + i, MIL_TEXT("PixelFormat"),
                M_TYPE_MIL_INT, &Len);
            PixelFormats[i] = new MIL_TEXT_CHAR[Len];
            MdigInquireFeature(MilDigitizer, M_FEATURE_ENUM_ENTRY_NAME + i, MIL_TEXT("PixelFormat"),
                M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(Len), PixelFormats[i]);
        }
    }

    for (MIL_INT i = 0; i < PixFrmtCount; i++)
    {
        if (MosStrcmp(MIL_TEXT("BayerRG8"), PixelFormats[i]) == 0)
        {
            /* Set pixel format to Raw8 (BayerRG8). */
            MdigControlFeature(
                MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("PixelFormat"), M_TYPE_STRING, MIL_TEXT("BayerRG8"));
        }
    }

    if (PixelFormats)
    {
        for (MIL_INT i = 0; i < PixFrmtCount; i++)
        {
            delete[] PixelFormats[i];
        }
        delete[] PixelFormats;
    }
}

/* Puts the camera back in default or if user set is not supported in non-triggered mode. */
void ResetTriggerControls(MIL_ID MilDigitizer)
{
    MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
    MIL_INT64 UserSetSelectorStringSize = 0;
    MdigInquireFeature(MilDigitizer, M_FEATURE_VALUE + M_STRING_SIZE, MIL_TEXT("UserSetSelector"), M_TYPE_MIL_INT,
        &UserSetSelectorStringSize);
    if (UserSetSelectorStringSize)
    {
        /* Set feature values to default */
        MdigControlFeature(
            MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("UserSetSelector"), M_TYPE_STRING, MIL_TEXT("Default"));
        MdigControlFeature(MilDigitizer, M_FEATURE_EXECUTE, MIL_TEXT("UserSetLoad"), M_DEFAULT, M_NULL);
    }
    else
    {
        MIL_INT TgSelCount = 0;
        MIL_TEXT_PTR* TriggerSelectors = NULL;
        MIL_INT Len;

        MdigInquireFeature(
            MilDigitizer, M_FEATURE_ENUM_ENTRY_COUNT, MIL_TEXT("TriggerSelector"), M_DEFAULT, &TgSelCount);
        if (TgSelCount)
        {
            TriggerSelectors = new MIL_TEXT_PTR[TgSelCount];
            for (MIL_INT i = 0; i < TgSelCount; i++)
            {
                MdigInquireFeature(MilDigitizer, M_FEATURE_ENUM_ENTRY_NAME + M_STRING_SIZE + i,
                    MIL_TEXT("TriggerSelector"), M_DEFAULT, &Len);
                TriggerSelectors[i] = new MIL_TEXT_CHAR[Len];
                MdigInquireFeature(MilDigitizer, M_FEATURE_ENUM_ENTRY_NAME + i, MIL_TEXT("TriggerSelector"), M_DEFAULT,
                    TriggerSelectors[i]);

                MdigControlFeature(
                    MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerSelector"), M_TYPE_STRING, TriggerSelectors[i]);
                MdigControlFeature(
                    MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerMode"), M_TYPE_STRING, MIL_TEXT("Off"));
            }
        }
    }
    MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);
}