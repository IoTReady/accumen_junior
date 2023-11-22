/*!
 * \file    lego_trigger.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-08-19
 * \since   1.0.0
 *
 * \brief   This application demonstrates how to use the device manager to open a camera,
 *          how to set parameters for triggering and how to display the first pixel value
 *          using the IDS peak IPL.
 *
 * \version 1.3.0
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

#define VERSION "1.3.0"

#include <cstdint>
#include <iostream>
#include <math.h>
#include <thread>

#include <peak_ipl/peak_ipl.hpp>
#include <peak/converters/peak_buffer_converter_ipl.hpp>
#include <peak/peak.hpp>

#include "softwaretriggerworker.h"


/*! \brief Wait for enter function
 *
 * The function waits for the user pressing the enter key.
 *
 * This function is called from main() whenever the program exits,
 * either in consequence of an error or after normal termination.
 */
void wait_for_enter();

/*! \brief Load UserSet Default function
 *
 * The function loads the UserSet Default.
 *
 * This function is called from main() whenever the program needs
 * to load UserSet Default, either on program start or in consequence
 * of an error or after normal termination.
 */
void load_userset_default(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice);

/*! \brief Get Available Trigger Cases function
 *
 * The function finds the available trigger cases.
 *
 * This function is called from main() when the program needs
 * to find available trigger cases.
 */
std::vector<char> getAvailableTriggerCases(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice);

/*! \brief Get feature availability function
 *
 * Returns if the feature with the given name is available or not.
 */
bool isFeatureAvailable(std::shared_ptr<peak::core::NodeMap>& nodeMap, const std::string& name);

/*! \brief Get feature entry availability function
 *
 * Returns if the enumeration feature with the given name is available and if it has an entry with the given name that is available.
 */
bool isFeatureEntryAvailable(
    std::shared_ptr<peak::core::NodeMap>& nodeMap, const std::string& name, const std::string& entry);

/*! \brief Get Available Trigger Sources function
 *
 * The function finds the available trigger sources for a given trigger source node.
 *
 * This function is called from main() when the program needs
 * to find available sources for a given trigger source node.
 */
std::vector<char> getAvailableTriggerSources(
    std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice, std::string triggerSourceNode);

/*! \brief Access Is Available function
 *
 * The function returns true if the access is available (readable and or writeable). 
 * Otherwise returns false.
 *
 * This function is called from main() whenever the program needs
 * to get the availability of an access status.
 */
bool access_is_available(peak::core::nodes::NodeAccessStatus access);

/*! \brief Find First Available function
 *
 * The function returns the first available entry (readable and or writeable)
 * of the given node. Otherwise returns an empty string.
 *
 * This function is called from main() whenever the program needs
 * to find out capabilities of an enumeration node.
 */
std::string find_first_available(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice,
    std::string enumerationNode, std::vector<std::string> entries);

/*! \brief Line Is Input function
 *
 * The function returns true if the given line can be used as input, 
 * otherwise returns false.
 *
 * This function is called from main() whenever the program needs
 * to find out input capabilities of a line.
 */
bool line_is_input(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice, std::string line);


int main()
{
    std::cout << "IDS peak API \"lego_trigger\" Sample v" << VERSION << std::endl;

    // initialize peak library
    peak::Library::Initialize();

    // create a device manager object
    auto& deviceManager = peak::DeviceManager::Instance();

    try
    {
        // update the deviceManager
        deviceManager.Update();

        // exit program if no device was found
        if (deviceManager.Devices().empty())
        {
            std::cout << "No device found. Exiting program." << std::endl << std::endl;
            wait_for_enter();
            // close peak library
            peak::Library::Close();
            return 0;
        }

        // list all available devices
        uint64_t i = 0;
        std::cout << "Devices available: " << std::endl;
        for (const auto& deviceDescriptor : deviceManager.Devices())
        {
            std::cout << i << ": " << deviceDescriptor->ModelName() << " ("
                      << deviceDescriptor->ParentInterface()->DisplayName() << "; "
                      << deviceDescriptor->ParentInterface()->ParentSystem()->DisplayName() << " v."
                      << deviceDescriptor->ParentInterface()->ParentSystem()->Version() << ")" << std::endl;
            ++i;
        }

        // select a device to open
        size_t selectedDevice = 0;
        // select a device to open via user input or remove these lines to always open the first available device
        std::cout << std::endl << "Select device to open: ";
        std::cin >> selectedDevice;

        // open the selected device
        auto device =
            deviceManager.Devices().at(selectedDevice)->OpenDevice(peak::core::DeviceAccessType::Control);
        // get the remote device node map
        auto nodeMapRemoteDevice = device->RemoteDevice()->NodeMaps().at(0);

        std::shared_ptr<peak::core::DataStream> dataStream;
        try
        {
            // Open standard data stream
            dataStream = device->DataStreams().at(0)->OpenDataStream();
        }
        catch (const std::exception& e)
        {
            // Open data stream failed
            device.reset();
            std::cout << "Failed to open DataStream: " << e.what() << std::endl;
            std::cout << "Exiting program." << std::endl << std::endl;

            wait_for_enter();
            // close library before exiting program
            peak::Library::Close();
            return 0;
        }

        // general preparations for triggered image acquisition
        // load the default user set, if available, to reset the device to a defined parameter set
        load_userset_default(nodeMapRemoteDevice);

        // allocate and announce image buffers
        auto payloadSize = nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("PayloadSize")
                               ->Value();
        auto bufferCountMax = dataStream->NumBuffersAnnouncedMinRequired();
        for (size_t bufferCount = 0; bufferCount < bufferCountMax; ++bufferCount)
        {
            auto buffer = dataStream->AllocAndAnnounceBuffer(static_cast<size_t>(payloadSize), nullptr);
            dataStream->QueueBuffer(buffer);
        }

        // Disable trigger temporarily if already selected
        if (nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                ->CurrentEntry()
                ->StringValue()
            == "On")
        {
            nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                ->SetCurrentEntry("Off");
        }
        // set a frame rate to 10fps (or max value) since some of the trigger cases require a defined frame rate
        auto frameRateMax = nodeMapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
                                ->Maximum();
        nodeMapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
            ->SetValue(std::min(10.0, frameRateMax));

        // check for trigger 'ExposureStart', 'FrameStart' or 'ReadoutStart'
        std::string imageTriggerStart = find_first_available(
            nodeMapRemoteDevice, "TriggerSelector", { "ExposureStart", "FrameStart", "ReadOutStart" });
        std::string imageTriggerEnd = find_first_available(
            nodeMapRemoteDevice, "TriggerSelector", { "ExposureEnd", "FrameEnd" });

        // user input to choose trigger case
        char triggerCase = ' ';
        auto triggerCases = getAvailableTriggerCases(nodeMapRemoteDevice);
        while (std::find(triggerCases.begin(), triggerCases.end(), triggerCase) >= triggerCases.end())
        {
            std::cout << std::endl << "Choose one of the following trigger cases: " << std::endl;
            for (const auto& trigger : triggerCases)
            {
                switch (trigger)
                {
                case 'b':
                    std::cout << "'b': \t basic, non-cyclic image acquisition" << std::endl;
                    break;
                case 'd':
                    std::cout << "'d': \t delayed, non-cyclic image acquisition" << std::endl;
                    break;
                case 's':
                    std::cout << "'s': \t scaled, non-cyclic image acquisition with trigger divider" << std::endl;
                    break;
                case 'a':
                    std::cout << "'a': \t acquisition period trigger" << std::endl;
                    break;
                case 'e':
                    std::cout << "'e': \t triggered exposure time" << std::endl;
                    break;
                case 'n':
                    std::cout << "'n': \t n images in fastest possible counted image sequence" << std::endl;
                    break;
                case 'm':
                    std::cout << "'m': \t m images in counted image sequence with fixed frame rate" << std::endl;
                    break;
                case 'p':
                    std::cout << "'p': \t p images in fastest possible time-controlled image sequence" << std::endl;
                    break;
                case 'q':
                    std::cout << "'q': \t q images in time-controlled image sequence with fixed frame rate" << std::endl;
                    break;
                default:
                    break;
                }
            }
            std::cin >> triggerCase;
        }

        std::string triggerSourceNode = "TriggerSource";
        if (triggerCase == 'n')
        {
            triggerSourceNode = "CounterTriggerSource";
        }
        else if (triggerCase == 'p')
        {
            triggerSourceNode = "TimerTriggerSource";
        }



        // user input to choose trigger source
        char triggerSource = ' ';
        std::vector<char> triggerSources = getAvailableTriggerSources(nodeMapRemoteDevice, triggerSourceNode);
        while (std::find(triggerSources.begin(), triggerSources.end(), triggerSource) >= triggerSources.end())
        {
            std::cout << std::endl << "Choose one of the following trigger sources: " << std::endl;
            for (const auto& ts : triggerSources)
            {
                switch (ts)
                {
                case 's':
                    std::cout << "'s': \t software trigger" << std::endl;
                    break;
                default:
                    std::cout << "'" << ts << "': \t hardware trigger on line " << ts << std::endl;
                }
            }
            std::cin >> triggerSource;
        }

        std::string triggerTypeStart = "";
        std::string triggerTypeEnd = "";

        auto softwareTriggerWorker = std::make_unique<SoftwareTriggerWorker>(nodeMapRemoteDevice);

        // do the parameter settings for the different trigger cases
        try
        {
            switch (triggerCase)
            {
            case 'b': // basic, non-cyclic image acquisition
                std::cout << "Parametrizing for trigger case 'basic'" << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                triggerTypeStart = imageTriggerStart;
                softwareTriggerWorker->setTriggerTypes(imageTriggerStart);
                break;
            case 'd': // delayed, non-cyclic image acquisition
                std::cout << "Parametrizing for trigger case 'delayed'" << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("TriggerDelay")
                    ->SetValue(100000.0);
                triggerTypeStart = imageTriggerStart;
                softwareTriggerWorker->setTriggerTypes(imageTriggerStart);
                break;
            case 's': // scaled, non-cyclic image acquisition with trigger divider
                std::cout << "Parametrizing for trigger case 'scaled'" << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("TriggerDivider")->SetValue(3);
                triggerTypeStart = imageTriggerStart;
                softwareTriggerWorker->setTriggerTypes(imageTriggerStart);
                break;
            case 'a': // acquisition period trigger
                std::cout << "Parametrizing for trigger case 'acquisition period'" << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry("AcquisitionStart");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                triggerTypeStart = "AcquisitionStart";
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry("AcquisitionEnd");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                triggerTypeEnd = "AcquisitionEnd";
                softwareTriggerWorker->setTriggerTypes("AcquisitionStart", "AcquisitionEnd");
                break;
            case 'e': // triggered exposure time
                std::cout << "Parametrizing for trigger case 'triggered exposure time'" << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                triggerTypeStart = imageTriggerStart;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(imageTriggerEnd);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                triggerTypeEnd = imageTriggerEnd;
                softwareTriggerWorker->setTriggerTypes(imageTriggerStart, imageTriggerEnd);
                break;
            case 'n': // n images in fastest possible counted image sequence
                std::cout << "Parametrizing for trigger case 'n images in fastest possible counted image sequence'"
                          << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                    ->SetCurrentEntry("Counter0Active");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerActivation")
                    ->SetCurrentEntry("LevelHigh");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterSelector")
                    ->SetCurrentEntry("Counter0");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterEventSource")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterEventActivation")
                    ->SetCurrentEntry("RisingEdge");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("CounterDuration")->SetValue(3);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterResetSource")
                    ->SetCurrentEntry("CounterTrigger");
                triggerTypeStart = "Counter0";
                softwareTriggerWorker->setTriggerTypes("Counter0");
                break;
            case 'm': // m images in counted image sequence with fixed frame rate
                std::cout << "Parametrizing for trigger case 'm images in counted image sequence with fixed frame rate'"
                          << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry("AcquisitionStart");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                triggerTypeStart = "AcquisitionStart";
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry("AcquisitionEnd");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                    ->SetCurrentEntry("Counter0End");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterSelector")
                    ->SetCurrentEntry("Counter0");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterTriggerSource")
                    ->SetCurrentEntry("AcquisitionStart");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterEventSource")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("CounterDuration")->SetValue(3);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterResetSource")
                    ->SetCurrentEntry("CounterTrigger");
                softwareTriggerWorker->setTriggerTypes("AcquisitionStart");
                break;
            case 'p': // p images in fastest possible time-controlled image sequence
                std::cout
                    << "Parametrizing for trigger case 'p images in fastest possible time-controlled image sequence'"
                    << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(imageTriggerStart);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                    ->SetCurrentEntry("Timer0Active");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerActivation")
                    ->SetCurrentEntry("LevelHigh");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerSelector")
                    ->SetCurrentEntry("Timer0");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("TimerDuration")
                    ->SetValue(500000.0);
                triggerTypeStart = "Timer0";
                softwareTriggerWorker->setTriggerTypes("Timer0");
                break;
            case 'q': // q images in time-controlled image sequence with fixed frame rate
                std::cout << "Parametrizing for trigger case 'q images in time-controlled image sequence with fixed "
                             "frame rate'"
                          << std::endl;
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry("AcquisitionStart");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                triggerTypeStart = "AcquisitionStart";
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry("AcquisitionEnd");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerMode")
                    ->SetCurrentEntry("On");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                    ->SetCurrentEntry("Timer0End");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerSelector")
                    ->SetCurrentEntry("Timer0");
                nodeMapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("TimerDuration")
                    ->SetValue(500000.0);
                nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerTriggerSource")
                    ->SetCurrentEntry("AcquisitionStart");
                softwareTriggerWorker->setTriggerTypes("AcquisitionStart");
                break;
            }

            switch (triggerSource)
            {
            case 's': // software trigger
                std::cout << "Parametrizing for software trigger " << std::endl;
                if (triggerTypeStart == "Counter0")
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterSelector")
                        ->SetCurrentEntry("Counter0");
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterResetSource")
                        ->SetCurrentEntry("Off");
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterTriggerSource")
                        ->SetCurrentEntry("Off");
                }
                else if (triggerTypeStart == "Timer0")
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerSelector")
                        ->SetCurrentEntry("Timer0");
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerTriggerSource")
                        ->SetCurrentEntry("Off");
                }
                else
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                        ->SetCurrentEntry(triggerTypeStart);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                        ->SetCurrentEntry("Software");
                }
                if (!triggerTypeEnd.empty())
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                        ->SetCurrentEntry(triggerTypeEnd);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                        ->SetCurrentEntry("Software");
                }
                break;
            default:
                std::string lineIn = "Line";
                lineIn.push_back(triggerSource);
                std::cout << "Parametrizing for hardware trigger on " << lineIn << std::endl;
                if (triggerTypeStart == "Counter0")
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterSelector")
                        ->SetCurrentEntry("Counter0");
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterTriggerSource")
                        ->SetCurrentEntry(lineIn);
                    nodeMapRemoteDevice
                        ->FindNode<peak::core::nodes::EnumerationNode>("CounterTriggerActivation")
                        ->SetCurrentEntry("RisingEdge");
                }
                else if (triggerTypeStart == "Timer0")
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerSelector")
                        ->SetCurrentEntry("Timer0");
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerTriggerSource")
                        ->SetCurrentEntry(lineIn);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TimerTriggerActivation")
                        ->SetCurrentEntry("RisingEdge");
                }
                else
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                        ->SetCurrentEntry(triggerTypeStart);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                        ->SetCurrentEntry(lineIn);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerActivation")
                        ->SetCurrentEntry("RisingEdge");
                }
                if (!triggerTypeEnd.empty())
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                        ->SetCurrentEntry(triggerTypeEnd);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSource")
                        ->SetCurrentEntry(lineIn);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerActivation")
                        ->SetCurrentEntry("FallingEdge");
                }

                // Make sure the selected line is an input line
                try
                {
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("LineSelector")
                        ->SetCurrentEntry(lineIn);
                    nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("LineMode")
                        ->SetCurrentEntry("Input");
                }
                catch (...)
                {}
                break;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << std::endl;
            std::cout << "EXCEPTION: " << e.what() << std::endl << std::endl;

            std::cout << "Failed to set parameters for selected trigger case '" << triggerCase
                      << "' with trigger source '" << triggerSource << "'" << std::endl;
            std::cout << "Most likely the camera doesn't support the selected trigger settings." << std::endl;

            // restore defined camera configuration by loading UserSet Default
            std::cout << "Loading UserSet Default to restore consistent camera configuration." << std::endl;
            load_userset_default(nodeMapRemoteDevice);

            wait_for_enter();
            // close library before exiting program
            peak::Library::Close();
            return 0;
        }


        // define stop condition for the program (number of images to acquire or duration of the acquisition period)
        uint64_t imageCountMax = 10000;
        uint64_t acquisitionTimeMax_s = 10;
        switch (triggerCase)
        {
        case 'a':
            // get number of seconds for acquisition duration via user input
            std::cout << std::endl << "Enter number of seconds for acquisition duration: ";
            std::cin >> acquisitionTimeMax_s;
            // imageCountMax should be higher than acquisitionTimeMax * frameRate
            imageCountMax = acquisitionTimeMax_s * 20;
            break;
        default:
            // get number of images to acquire via user input
            std::cout << std::endl << "Enter number of images to acquire: ";
            std::cin >> imageCountMax;
            break;
        }

        // Lock critical features to prevent them from changing during acquisition
        nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("TLParamsLocked")->SetValue(1);

        // start acquisition
        dataStream->StartAcquisition();
        nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStart")->Execute();
        nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStart")->WaitUntilDone();

        // if software trigger is enabled, start a separate thread to do the triggering
        if (triggerSource == 's')
        {
            switch (triggerCase)
            {
            case 'a':
                softwareTriggerWorker->setSleepTimes(acquisitionTimeMax_s * 1000);
                softwareTriggerWorker->start();
                break;
            case 'e':
                softwareTriggerWorker->setSleepTimes(0, 500);
                softwareTriggerWorker->start();
                break;
            case 'n':
            case 'm':
            case 'p':
            case 'q':
                softwareTriggerWorker->setSleepTimes(1000);
                softwareTriggerWorker->start();
                break;
            default:
                softwareTriggerWorker->setSleepTimes(100);
                softwareTriggerWorker->start();
                break;
            }
        }

        // process the acquired images
        uint64_t imageCount = 0;
        std::cout << std::endl << "First pixel value of each image: " << std::endl;
        while ((imageCount < imageCountMax) && softwareTriggerWorker->isTriggerActive())
        {
            // get buffer from datastream and create IDS peak IPL image from it
            auto buffer = dataStream->WaitForFinishedBuffer(5000);
            auto image = peak::BufferTo<peak::ipl::Image>(buffer);

            // output first pixel value
            std::cout << static_cast<uint16_t>(*image.PixelPointer(0, 0)) << " ";

            // queue buffer
            dataStream->QueueBuffer(buffer);
            ++imageCount;
        }
        std::cout << std::endl << std::endl;

        // stop software trigger task
        softwareTriggerWorker->stop();

        // stop acquistion of device
        nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStop")->Execute();
        nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStop")->WaitUntilDone();
        dataStream->StopAcquisition(peak::core::AcquisitionStopMode::Default);

        // Unlock parameters after acquisition stop
        nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("TLParamsLocked")->SetValue(0);

        // flush and revoke all buffers
        dataStream->Flush(peak::core::DataStreamFlushMode::DiscardAll);
        for (const auto& buffer : dataStream->AnnouncedBuffers())
        {
            dataStream->RevokeBuffer(buffer);
        }

        // ask, if camera configuration should be kept or UserSet Default should be loaded
        std::cout << "Closing the application.\n"
                     "Do you want to keep the trigger configuration or to restore default parameters (UserSet Default)."
                  << std::endl;
        std::cout << "'y': \t Keep trigger configuration" << std::endl;
        std::cout << "'n': \t Do not keep trigger configuration, restore default parameters. (or any other key)"
                  << std::endl;
        char selection = ' ';
        std::cin >> selection;

        switch (selection)
        {
        case 'y':
        case 'Y':
            std::cout << "Parameters will be kept." << std::endl;
            break;
        default:
            std::cout << "Restoring default parameters (load UserSet Default)." << std::endl;
            load_userset_default(nodeMapRemoteDevice);
            break;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    }

    wait_for_enter();
    // close library before exiting program
    peak::Library::Close();
    return 0;
}


void wait_for_enter()
{
    std::cout << std::endl;
#if defined(WIN32)
    system("pause");
#elif defined(__linux__)
    std::cout << "Press enter to exit." << std::endl;
    system("read _");
#else
#    warning("Operating system not implemented!")
#endif
}


void load_userset_default(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice)
{
    try
    {
        nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("UserSetSelector")
            ->SetCurrentEntry("Default");
        nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("UserSetLoad")->Execute();
        // wait until the UserSetLoad command has been finished
        nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("UserSetLoad")->WaitUntilDone();
    }
    catch (const peak::core::NotFoundException&)
    {
        // UserSet is not available
        std::cout << "WARNING: Failed to load UserSet Default." << std::endl;
    }
}

std::vector<char> getAvailableTriggerCases(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice)
{
    std::vector<char> triggerCases = { 'b', 'd', 's', 'a', 'e', 'n', 'm', 'p', 'q' };
    std::vector<char> result;

    std::string imageTriggerStart = find_first_available(
        nodeMapRemoteDevice, "TriggerSelector", { "ExposureStart", "FrameStart", "ReadOutStart" });
    std::string imageTriggerEnd = find_first_available(
        nodeMapRemoteDevice, "TriggerSelector", { "ExposureEnd", "FrameEnd"});

    for (const auto& triggerCase : triggerCases)
    {
        switch (triggerCase)
        {
        case 'b': // basic, non-cyclic image acquisition
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 'd': // delayed, non-cyclic image acquisition
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On")
                && isFeatureAvailable(nodeMapRemoteDevice, "TriggerDelay"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 's': // scaled, non-cyclic image acquisition with trigger divider
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On")
                && isFeatureAvailable(nodeMapRemoteDevice, "TriggerDivider"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 'a': // acquisition period trigger
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", "AcquisitionStart")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", "AcquisitionEnd")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 'e': // triggered exposure time
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", imageTriggerEnd)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 'n': // n images in fastest possible counted image sequence
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSource", "Counter0Active")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "CounterEventSource", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "CounterEventActivation", "RisingEdge")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "CounterResetSource", "CounterTrigger")
                && isFeatureAvailable(nodeMapRemoteDevice, "CounterDuration"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 'm': // m images in counted image sequence with fixed frame rate
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", "AcquisitionStart")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", "AcquisitionEnd")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSource", "Counter0End")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "CounterTriggerSource", "AcquisitionStart")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "CounterEventSource", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "CounterResetSource", "CounterTrigger")
                && isFeatureAvailable(nodeMapRemoteDevice, "CounterDuration"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 'p': // p images in fastest possible time-controlled image sequence
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", imageTriggerStart)
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSource", "Timer0Active")
                && isFeatureAvailable(nodeMapRemoteDevice, "TimerDuration"))
            {
                result.emplace_back(triggerCase);
            }
            break;
        case 'q': // q images in time-controlled image sequence with fixed frame rate
            if (isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", "AcquisitionStart")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSelector", "AcquisitionEnd")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerMode", "On")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TriggerSource", "Timer0End")
                && isFeatureEntryAvailable(nodeMapRemoteDevice, "TimerTriggerSource", "AcquisitionStart")
                && isFeatureAvailable(nodeMapRemoteDevice, "TimerDuration"))
            {
                result.emplace_back(triggerCase);
            }
        }
    }
    return result;
}

bool isFeatureAvailable(std::shared_ptr<peak::core::NodeMap>& nodeMap, const std::string& name)
{
    try
    {
        if (nodeMap->HasNode(name))
        {
            auto access = nodeMap->FindNode(name)->AccessStatus();
            return access_is_available(access);
        }
        else
        {
            return false;
        }
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool isFeatureEntryAvailable(
    std::shared_ptr<peak::core::NodeMap>& nodeMap, const std::string& featureName, const std::string& entryName)
{
    try
    {
        if (nodeMap->HasNode(featureName))
        {
            auto access = nodeMap->FindNode<peak::core::nodes::EnumerationNode>(featureName)
                              ->FindEntry(entryName)->AccessStatus();
            return access_is_available(access);
        }
        else
        {
            return false;
        }
    }
    catch (const std::exception&)
    {
        return false;
    }
}

std::vector<char> getAvailableTriggerSources(
    std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice, std::string triggerSourceNode)
{
    std::vector<char> result;
    std::shared_ptr<peak::core::nodes::EnumerationNode> node;
    try
    {
        node = nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>(triggerSourceNode);
    }
    catch (...)
    {
        return result;
    }

    for (const auto& entry : node->Entries())
    {
        try
        {
            if (entry->SymbolicValue() == "Software")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access))
                {
                    result.emplace_back('s');
                }
            }
            else if (entry->SymbolicValue() == "Line0")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line0"))
                {
                    result.emplace_back('0');
                }
            }
            else if (entry->SymbolicValue() == "Line1")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line1"))
                {
                    result.emplace_back('1');
                }
            }
            else if (entry->SymbolicValue() == "Line2")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line2"))
                {
                    result.emplace_back('2');
                }
            }
            else if (entry->SymbolicValue() == "Line3")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line3"))
                {
                    result.emplace_back('3');
                }
            }
            else if (entry->SymbolicValue() == "Line4")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line4"))
                {
                    result.emplace_back('4');
                }
            }
            else if (entry->SymbolicValue() == "Line5")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line5"))
                {
                    result.emplace_back('5');
                }
            }
            else if (entry->SymbolicValue() == "Line6")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line6"))
                {
                    result.emplace_back('6');
                }
            }
            else if (entry->SymbolicValue() == "Line7")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line7"))
                {
                    result.emplace_back('7');
                }
            }
            else if (entry->SymbolicValue() == "Line8")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line8"))
                {
                    result.emplace_back('8');
                }
            }
            else if (entry->SymbolicValue() == "Line9")
            {
                auto access = entry->AccessStatus();
                if (access_is_available(access) && line_is_input(nodeMapRemoteDevice, "Line9"))
                {
                    result.emplace_back('9');
                }
            }
        }
        catch (...)
        {
            // Continue without error
        }
    }

    return result;
}


bool access_is_available(peak::core::nodes::NodeAccessStatus access)
{
    if ((access == peak::core::nodes::NodeAccessStatus::ReadWrite)
        || (access == peak::core::nodes::NodeAccessStatus::ReadOnly))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool line_is_input(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice, std::string line)
{ 
    try
    {
        nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("LineSelector")
            ->SetCurrentEntry(line);
        if (nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("LineMode")
                ->CurrentEntry()->SymbolicValue() == "Input")
        {
            return true;
        }
        nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("LineMode")
            ->SetCurrentEntry("Input");
        return true;
    }
    catch (...)
    {}
    
    return false;
}

std::string find_first_available(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice,
    std::string enumerationNode, std::vector<std::string> entries)
{
    std::shared_ptr<peak::core::nodes::EnumerationNode> node;
    try
    {
        node = nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>(enumerationNode);
    }
    catch (...)
    {
        return "";
    }

    for (const auto& entry : entries)
    {
        try
        {
            auto access = node->FindEntry(entry)->AccessStatus();
            if (access_is_available(access))
            {
                return entry;
            }
        }
        catch (...)
        {
            // Continue without error
        }
    }

    return "";
}