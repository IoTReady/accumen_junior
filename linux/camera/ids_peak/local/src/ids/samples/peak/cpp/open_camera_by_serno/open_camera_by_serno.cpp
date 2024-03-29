/*!
 * \file    open_camera_by_serno.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2021-03-05
 * \since   1.0.0
 *
 * \brief   This application demonstrates how to use the device manager to open a camera
 *          by its serial number using a specific CTI
 *
 * \version 1.1.0
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

#define VERSION "1.1.1"

#include <cstddef>
#include <iostream>

#include <peak/peak.hpp>


/*! \bief Wait for enter function
 *
 * The function waits for the user pressing the enter key.
 *
 * This function is called from main() whenever the program exits,
 * either in consequence of an error or after normal termination.
 */
void wait_for_enter();

int main()
{
    std::cout << "IDS peak API \"open_camera_by_serno\" Sample v" << VERSION << std::endl;

    // initialize peak library
    peak::Library::Initialize();

    // create a device manager object
    auto& deviceManager = peak::DeviceManager::Instance();

    try
    {
        // This sample requires that the CTI returns the serial number of a device without
        // throwing an exception. The following section adds only the suitable CTIs to
        // the device manager. If you are sure that all CTIs return a serial number, you can
        // remove this section and use 'deviceManager.Update()' without update policy instead.

        {
            std::vector<std::string> ctiPaths;
            deviceManager.Update();

            // exit program if no device was found
            if (deviceManager.Devices().empty())
            {
                std::cout << "No device found. Exiting program." << std::endl << std::endl;
                wait_for_enter();
                // close library before exiting program
                peak::Library::Close();
                return 0;
            }

            for (const auto& deviceDescriptor : deviceManager.Devices())
            {
                try
                {
                    deviceDescriptor->SerialNumber();
                    ctiPaths.emplace_back(deviceDescriptor->ParentInterface()->ParentSystem()->CTIFullPath());
                }
                catch (const std::exception&)
                {
                    // do not add CTI to device manager
                    std::cout << "Info: '" << deviceDescriptor->ParentInterface()->ParentSystem()->DisplayName()
                              << "' will be skipped." << std::endl;
                }
            }
            std::cout << std::endl;

            // reset the deviceManager to clear out all found CTIs, so only the selected ones will be used below
            deviceManager.Reset();
            for (auto& ctiPath : ctiPaths)
            {
                deviceManager.AddProducerLibrary(ctiPath);
            }
        }

        // update the deviceManager without touching the CTIs
        deviceManager.Update(peak::DeviceManager::UpdatePolicy::DontScanEnvironmentForProducerLibraries);

        // exit program if no device was found
        if (deviceManager.Devices().empty())
        {
            std::cout << "No device found. Exiting program." << std::endl << std::endl;
            wait_for_enter();
            // close library before exiting program
            peak::Library::Close();
            return 0;
        }

        // define the serial number of the device to be opened
        std::string serialNumber = "4103130819";
        // remove the following section to use the hard coded serial number instead of the user selected serial number
        {
            std::vector<std::string> serialNumbers;
            std::cout << "Serial numbers available: " << std::endl;
            size_t i = 0;
            for (const auto& deviceDescriptor : deviceManager.Devices())
            {
                // Check for duplicates
                if (std::find(serialNumbers.begin(), serialNumbers.end(), deviceDescriptor->SerialNumber())
                    == serialNumbers.end())
                {
                    std::cout << i << ": \t" << deviceDescriptor->SerialNumber() << " - "
                              << deviceDescriptor->DisplayName() << std::endl;
                    serialNumbers.push_back(deviceDescriptor->SerialNumber());
                    ++i;
                }
            }

            // select serial number
            std::cout << std::endl << "Select serial number index [0-" << deviceManager.Devices().size() - 1 << "]: ";
            std::cin >> i;

            if (std::cin.fail())
            {
                std::cout << "Invalid input! Using index 0." << std::endl;
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                i = 0;
            }

            if (i >= serialNumbers.size())
            {
                std::cout << "Invalid index! Using index 0." << std::endl;
                i = 0;
            }

            serialNumber = serialNumbers.at(i);
        }

        // check devices for serial number and cti and remember their index
        std::vector<size_t> deviceList;
        size_t i = 0;
        std::cout << "Device with serial number " << serialNumber
                  << " can be opened with the following CTIs: " << std::endl;
        for (const auto& deviceDescriptor : deviceManager.Devices())
        {
            if (deviceDescriptor->SerialNumber() == serialNumber)
            {
                std::cout << deviceList.size() << ": \t"
                          << deviceDescriptor->ParentInterface()->ParentSystem()->DisplayName() << " v."
                          << deviceDescriptor->ParentInterface()->ParentSystem()->Version() << std::endl;
                deviceList.push_back(i);
            }
            ++i;
        }

        // if only one CTI is available, open it, otherwise let the user select one CTI
        size_t selectedDevice = deviceList.at(0);
        if (deviceList.size() > 1)
        {
            // select CTI
            std::cout << std::endl << "Select CTI: ";
            std::cin >> i;

            if (std::cin.fail())
            {
                std::cout << "Invalid input! Using index 0." << std::endl;
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                selectedDevice = deviceList.at(0);
            }

            if (selectedDevice >= deviceManager.Devices().size())
            {
                std::cout << "Invalid index! Using index 0." << std::endl;
                selectedDevice = deviceList.at(0);
            }
            else
            {
                selectedDevice = deviceList.at(i);
            }
        }

        auto device = deviceManager.Devices().at(selectedDevice)->OpenDevice(peak::core::DeviceAccessType::Control);

        // get the remote device node map
        auto nodeMapRemoteDevice = device->RemoteDevice()->NodeMaps().at(0);

        try
        {
            // print model name, not knowing if device has the node "DeviceModelName"
            std::cout
                << "Model Name: "
                << nodeMapRemoteDevice->FindNode<peak::core::nodes::StringNode>("DeviceModelName")->Value()
                << std::endl;
        }
        catch (const std::exception&)
        {
            // if "DeviceModelName" is not a valid node name, do error handling here...
            std::cout << "Model Name: (unknown)" << std::endl;
        }

        try
        {
            // print user ID, not knowing if device has the node "DeviceUserID"
            std::cout << "User ID: "
                      << nodeMapRemoteDevice->FindNode<peak::core::nodes::StringNode>("DeviceUserID")->Value()
                      << std::endl;
        }
        catch (const std::exception&)
        {
            // if "DeviceUserID" is not a valid node name, do error handling here...
            std::cout << "User ID: (unknown)" << std::endl;
        }

        try
        {
            // print sensor information, not knowing if device has the node "SensorName"
            std::cout << "Sensor Name: "
                      << nodeMapRemoteDevice->FindNode<peak::core::nodes::StringNode>("SensorName")->Value()
                      << std::endl;
        }
        catch (const std::exception&)
        {
            // if "SensorName" is not a valid node name, do error handling here...
            std::cout << "Sensor Name: (unknown)" << std::endl;
        }

        // print resolution
        std::cout << "Max. resolution (w x h): "
                  << nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("WidthMax")->Value() << " x "
                  << nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("HeightMax")->Value() << std::endl;
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
#endif
}
