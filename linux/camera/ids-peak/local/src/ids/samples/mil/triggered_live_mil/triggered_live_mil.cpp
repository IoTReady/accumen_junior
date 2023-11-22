/*!
 * \file    triggered_live_mil.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0.0
 *
 * \brief   Simple example for grabbing images triggered via the GenICamTL interface
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

#include <mil.h>
#include <sstream>
#include <string>
#if M_MIL_USE_WINDOWS
#   include <windows.h>
#endif

using namespace std;
/* Define a STL compatible string that maps to MIL strings. */
typedef basic_string<MIL_TEXT_CHAR, char_traits<MIL_TEXT_CHAR>, allocator<MIL_TEXT_CHAR>> mil_string;


/* List of function prototypes used to perform triggered acquisition. */
typedef enum
{
    eSingleFrame = 1,
    eMultiFrame,
    eContinuous
} eTriggerType;
void IDS_software_trigger(MIL_ID MilDigitizer, MIL_ID MilImage, MIL_ID MilDisplay, MIL_INT* Selection);
void IDS_hardware_trigger(string Mode, string Activation, MIL_ID MilSystem, MIL_ID MilDigitizer, MIL_ID MilImageDisp,
    MIL_ID MilDisplay, MIL_INT* Selection);
void ResetTriggerControls(MIL_ID MilDigitizer);
void CanTriggerExposureStartInquiry(MIL_ID MilDigitizer);

/* Global variables used to store camera capabilities.*/
bool CanTriggerExposureStart = false;

/* Main function. */
int MosMain(void)
{
    MIL_ID MilApplication, /* Application identifier.  */
        MilSystem, /* System identifier.       */
        MilDisplay, /* Display identifier.      */
        MilDigitizer, /* Digitizer identifier.    */
        MilImage; /* Image buffer identifier. */
    MIL_INT BoardType;
    MIL_INT Selection;
    MIL_INT Mode;
    MIL_INT Activation;
    MIL_BOOL inputExpected = TRUE;

    /* Allocate defaults. */
    MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

    /* Allocate the digitizer controlling the camera. */
    MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);

    /* Allocate grab and display buffer. */
    MbufAllocColor(MilSystem, MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL),
        MdigInquire(MilDigitizer, M_SIZE_X, M_NULL), MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL),
        MdigInquire(MilDigitizer, M_TYPE, M_NULL), M_IMAGE + M_DISP + M_GRAB, &MilImage);

    /* Get information on the system we are using and print a welcome message to the console. */
    MsysInquire(MilSystem, M_BOARD_TYPE, &BoardType);

    if (((BoardType & M_BOARD_TYPE_MASK) != M_GIGE_VISION))
    {
        /* Print error message. */
        MosPrintf(
            MIL_TEXT("This example program can only be used with the Matrox Driver for ") MIL_TEXT("GigE Vision.\n"));
        MosPrintf(
            MIL_TEXT("Please ensure that the default system type is set accordingly in ") MIL_TEXT("MIL Config.\n"));
        MosPrintf(MIL_TEXT("-------------------------------------------------------------\n\n"));
        MosPrintf(MIL_TEXT("Press <enter> to quit.\n"));
        MosGetch();
        MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);
        return 1;
    }

    CanTriggerExposureStartInquiry(MilDigitizer);

    while (inputExpected)
    {
#if M_MIL_USE_WINDOWS
        system("cls");
#endif
        /* Print a message. */
        MosPrintf(MIL_TEXT("TRIGGER MODI EXAMPLE:\n\n"));
        MosPrintf(MIL_TEXT(
            "For freerun please select <f>, for software trigger <s>, hardware trigger <h> \nand <q> to quit.\n"));
        Selection = MosGetch();
        // Char = static_cast<MIL_TEXT_CHAR>(Selection);
        if (Selection == 'F' || Selection == 'f')
        {
            /* Start a continuous acquisition. */
            /* F R E E R U N */
            /* Print a message. */
            MosPrintf(MIL_TEXT("\nFreerun is selected, to start acquisition press <f>, \nto quit any other key.\n"));
            Selection = MosGetch();

            if (Selection == 'F' || Selection == 'f')
            {
                /* Set feature values to default */
                ResetTriggerControls(MilDigitizer);

                MdispSelect(MilDisplay, MilImage);
                MdigGrabContinuous(MilDigitizer, MilImage);

                /* Print a message. */
                MosPrintf(MIL_TEXT("\nContinuous image grab in progress.\n"));
                MosPrintf(MIL_TEXT("Press <Enter> to stop.\n"));
                MosGetch();

                /* Stop the continuous acquisition. */
                MdigHalt(MilDigitizer);

                /* Print a message. */
                MosPrintf(MIL_TEXT("\nPress <Enter> to start acquisition of 100 images.\n"));
                MosGetch();
                MosPrintf(MIL_TEXT("\nAcquisition of 100 images in process:\n"));
                for (MIL_INT i = 0; i < 100; i++)
                {
                    MdispSelect(MilDisplay, MilImage);
                    MdigGrab(MilDigitizer, MilImage);
                    MosPrintf(MIL_TEXT("."));
                }

                MosPrintf(MIL_TEXT("\n\nPress <q> to quit or any other key to continue.\n"));
                Selection = MosGetch();
            }
            else
            {
                Selection = 'q';
            }
        }
        else if (Selection == 'S' || Selection == 's')
        {
            /* S O F T W A R E T R I G G E R */
            /* Print a message. */
            MosPrintf(
                MIL_TEXT("\nSoftware trigger is selected, to start acquisition press <s>, \nto quit any other key.\n"));
            Selection = MosGetch();

            if (Selection == 'S' || Selection == 's')
            {
                IDS_software_trigger(MilDigitizer, MilImage, MilDisplay, &Selection);
            }
            else
            {
                Selection = 'q';
            }
        }
        else if (Selection == 'H' || Selection == 'h')
        {
            /*  H A R D W A R E T R I G G E R */
            /* Print a message. */
            MosPrintf(MIL_TEXT("\nHardware trigger selected.\n"));
            MosPrintf(MIL_TEXT("For Line 0 please select <0>, for Line 2 <2> and for Line 3 <3>.\n"));
            Mode = MosGetch();
            mil_string TriggerSource = "Line";
            if (Mode == '0' || Mode == '2' || Mode == '3')
            {
                std::stringstream ss;
                ss << "Line" << char(Mode);
                TriggerSource = ss.str();

                MosPrintf(MIL_TEXT("\nPlease specify the activation mode of the trigger, \nplease select Rising Edge "
                                   "<r> or Falling Edge <f>.\n"));
                Activation = MosGetch();
                if (Activation == 'r' || Activation == 'R')
                {
                    IDS_hardware_trigger(
                        TriggerSource, "RisingEdge", MilSystem, MilDigitizer, MilImage, MilDisplay, &Selection);
                }
                else if (Activation == 'f' || Activation == 'F')
                {
                    IDS_hardware_trigger(
                        TriggerSource, "FallingEdge", MilSystem, MilDigitizer, MilImage, MilDisplay, &Selection);
                }
                else
                {
                    Selection = 'q';
                }
            }
            else
            {
                Selection = 'q';
            }
        }
        if (Selection == 'Q' || Selection == 'q')
        {
            inputExpected = false;
            MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage);

            return 0;
        }
        else
        {
            inputExpected = true;
        }
    }

    MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage);

    return 0;
}


/* SOFTWARE TRIGGER */
void IDS_software_trigger(MIL_ID MilDigitizer, MIL_ID MilImage, MIL_ID MilDisplay, MIL_INT* Selection)
{
    /* Set feature values to default */
    ResetTriggerControls(MilDigitizer);

    /* Check, if the device supports Software Trigger. */
    if (CanTriggerExposureStart)
    {

        MdigControlFeature(
            MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerSelector"), M_TYPE_STRING, MIL_TEXT("ExposureStart"));
        MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerMode"), M_TYPE_STRING, MIL_TEXT("On"));
        MdigControlFeature(
            MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerSource"), M_TYPE_STRING, MIL_TEXT("Software"));

        /* Start a continuous acquisition. */
        MdispSelect(MilDisplay, MilImage);
        MdigGrabContinuous(MilDigitizer, MilImage);

        MosPrintf(MIL_TEXT("\nSoftware triggering in process:\n"));
        for (MIL_INT i = 0; i < 100; i++)
        {
            MdigControlFeature(MilDigitizer, M_FEATURE_EXECUTE, MIL_TEXT("TriggerSoftware"), M_DEFAULT, M_NULL);
            MdispSelect(MilDisplay, MilImage);
            /* The sleep is needed to show every image triggered by the SoftwareTrigger */
            MosPrintf(MIL_TEXT("."));
            Sleep(100);
        }
        /* Stop the processing. */
        MdigHalt(MilDigitizer);
    }
    else
    {
        MosPrintf(MIL_TEXT("\nPress any key to quit.\n"));
        MosGetch();
        *Selection = 'q';
    }
}
/* User's processing function hook data structure. */
typedef struct
{
    MIL_ID MilDigitizer;
    MIL_ID MilImageDisp;
    MIL_ID MilDisplay;
    MIL_INT ProcessedImageCount;
} HookDataStruct;

/* User's processing function prototype. */
MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

/* IDS HARDWARE TRIGGER */
void IDS_hardware_trigger(string TriggerSource, string Activation, MIL_ID MilSystem, MIL_ID MilDigitizer,
    MIL_ID MilImageDisp, MIL_ID MilDisplay, MIL_INT* Selection)
{
    MIL_INT Done = 0;
    MIL_TEXT_CHAR source[1024];
    MIL_TEXT_CHAR activation[1024];
    eTriggerType TriggerType = (eTriggerType)0;
    bool SoftwareTriggerSelected = false;
    MIL_INT NbFrames = 10;
    MIL_INT MilGrabBufferListSize;
    MIL_INT Ch = 0;
    MIL_ID* MilGrabBufferList = NULL;
    MIL_TEXT_CHAR TriggerSelector[256] = { '\0' };
    ;
    HookDataStruct UserHookData;

    MosPrintf(MIL_TEXT("\nHardware trigger with Triggersource = %s and TriggerActivation = %s selected.\n"),
        TriggerSource.c_str(), Activation.c_str());

    /* Set feature values to default */
    ResetTriggerControls(MilDigitizer);

    MdigControlFeature(
        MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerSelector"), M_TYPE_STRING, MIL_TEXT("ExposureStart"));
    MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerMode"), M_TYPE_STRING, MIL_TEXT("On"));

    strncpy_s(source, TriggerSource.c_str(), sizeof(source));
    source[sizeof(source) - 1] = 0;

    MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerSource"), M_TYPE_STRING, MIL_TEXT(source));

    strncpy_s(activation, Activation.c_str(), sizeof(activation));
    activation[sizeof(activation) - 1] = 0;

    MdigControlFeature(
        MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerActivation"), M_TYPE_STRING, MIL_TEXT(activation));
    MdigControlFeature(
        MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("AcquisitionMode"), M_TYPE_STRING, MIL_TEXT("Continuous"));

    /* set the ExpposureTime*/
    MIL_DOUBLE ExposureTimeInus = 2500.0;
    MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("ExposureTime"), M_TYPE_DOUBLE, &ExposureTimeInus);

    /* Set the grab timeout to infinite for triggered grab. */
    MdigControl(MilDigitizer, M_GRAB_TIMEOUT, M_INFINITE);

    MilGrabBufferList = new MIL_INT[NbFrames];

    /* Allocate the grab buffers and clear them. */
    MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
    for (MilGrabBufferListSize = 0; MilGrabBufferListSize < NbFrames; MilGrabBufferListSize++)
    {
        MbufAllocColor(MilSystem, MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL),
            MdigInquire(MilDigitizer, M_SIZE_X, M_NULL), MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL),
            MdigInquire(MilDigitizer, M_TYPE, M_NULL), M_IMAGE + M_GRAB + M_PROC,
            &MilGrabBufferList[MilGrabBufferListSize]);

        if (MilGrabBufferList[MilGrabBufferListSize])
        {
            MbufClear(MilGrabBufferList[MilGrabBufferListSize], 0xFF);
        }
        else
        {
            break;
        }
    }
    MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

    /* Initialize the User's processing function data structure. */
    UserHookData.MilDigitizer = MilDigitizer;
    UserHookData.MilImageDisp = MilImageDisp;
    UserHookData.MilDisplay = MilDisplay;
    UserHookData.ProcessedImageCount = 0;

    /* Set the grab timeout to infinite for triggered grab. */
    MdigControl(MilDigitizer, M_GRAB_TIMEOUT, M_INFINITE);

    /* Print a message and wait for a key press after a minimum number of frames. */
    MosPrintf(MIL_TEXT("\nWaiting for a input trigger signal.\n"));
    MosPrintf(MIL_TEXT("Press <q> to quit or any other key to get back.\n\n"));

    do
    {
        /* Start the processing. The processing function is called for every frame grabbed. */
        MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, M_START, M_ASYNCHRONOUS, ProcessingFunction,
            &UserHookData);

        Done = MosGetch();

        /* Stop the processing. */
        MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, Done ? M_STOP : M_STOP + M_WAIT, M_DEFAULT,
            ProcessingFunction, &UserHookData);
    } while (!Done);

    /* Reset the camera to non-triggered mode. */
    ResetTriggerControls(MilDigitizer);

    /* Free the grab buffers. */
    while (MilGrabBufferListSize > 0)
    {
        MbufFree(MilGrabBufferList[--MilGrabBufferListSize]);
    }
    delete[] MilGrabBufferList;

    *Selection = Done;
}

/* TriggerSelector ExposureStart is supported */
void CanTriggerExposureStartInquiry(MIL_ID MilDigitizer)
{
    MIL_TEXT_PTR* TriggerSelectors = NULL;
    MIL_INT TgSelCount = 0;
    MIL_INT Len;
    MdigInquireFeature(MilDigitizer, M_FEATURE_ENUM_ENTRY_COUNT, MIL_TEXT("TriggerSelector"), M_DEFAULT, &TgSelCount);
    if (TgSelCount)
    {
        TriggerSelectors = new MIL_TEXT_PTR[TgSelCount];
        for (MIL_INT i = 0; i < TgSelCount; i++)
        {
            MdigInquireFeature(MilDigitizer, M_FEATURE_ENUM_ENTRY_NAME + M_STRING_SIZE + i, MIL_TEXT("TriggerSelector"),
                M_DEFAULT, &Len);
            TriggerSelectors[i] = new MIL_TEXT_CHAR[Len];
            MdigInquireFeature(MilDigitizer, M_FEATURE_ENUM_ENTRY_NAME + i, MIL_TEXT("TriggerSelector"), M_DEFAULT,
                TriggerSelectors[i]);

            if (MosStrcmp(MIL_TEXT("ExposureStart"), TriggerSelectors[i]) == 0)
            {
                CanTriggerExposureStart = true;
            }
        }
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

/* User's processing function called every time a grab buffer is modified. */
/* -----------------------------------------------------------------------*/

/* Local defines. */
#define STRING_LENGTH_MAX 20
#define STRING_POS_X 20
#define STRING_POS_Y 20

MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
    HookDataStruct* UserHookDataPtr = (HookDataStruct*)HookDataPtr;
    MIL_ID ModifiedBufferId;
    MIL_TEXT_CHAR Text[STRING_LENGTH_MAX] = {
        MIL_TEXT('\0'),
    };

    ///* Retrieve the MIL_ID of the grabbed buffer. */
    MdigGetHookInfo(HookId, M_MODIFIED_BUFFER + M_BUFFER_ID, &ModifiedBufferId);

    ///* Print and draw the frame count. */
    UserHookDataPtr->ProcessedImageCount++;
    MosPrintf(MIL_TEXT("Processing frame #%d.\r"), UserHookDataPtr->ProcessedImageCount);
    MosSprintf(Text, STRING_LENGTH_MAX, MIL_TEXT("%ld"), UserHookDataPtr->ProcessedImageCount);
    MgraText(M_DEFAULT, ModifiedBufferId, STRING_POS_X, STRING_POS_Y, Text);

    ///* Perform the processing and update the display. */
    MdispSelect(UserHookDataPtr->MilDisplay, UserHookDataPtr->MilImageDisp);
    MbufCopy(ModifiedBufferId, UserHookDataPtr->MilImageDisp);

    return 0;
}
