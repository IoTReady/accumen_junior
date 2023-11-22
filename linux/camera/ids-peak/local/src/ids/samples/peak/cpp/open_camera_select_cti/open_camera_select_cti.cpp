/*!
 * \file    open_camera_select_cti.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2021-03-05
 * \since   1.0.0
 *
 * \brief   This application demonstrates how to use the device manager to open a camera
 *          using only a specific CTI
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
    std::cout << "IDS peak API \"open_camera_select_cti\" Sample v" << VERSION << std::endl;

    // initialize peak library
    peak::Library::Initialize();

    // create a device manager object
    auto& deviceManager = peak::DeviceManager::Instance();

    try
    {
        // define the CTI that will be used
        std::string cti = "";
        // Add a CTI path and remove the following section to use the hard coded CTI instead of the user selected CTI
        {
            deviceManager.Update();

            auto systems = deviceManager.Systems();

            // exit program if no device was found
            if (systems.empty())
            {
                std::cout << "No CTIs found. Exiting program." << std::endl << std::endl;
                wait_for_enter();
                // close library before exiting program
                peak::Library::Close();
                return 0;
            }

            std::cout << "CTIs available: " << std::endl;
            size_t i = 0;
            for (const auto& system : systems)
            {
                std::cout << i << ": \t" << system->DisplayName() << " v." << system->Version() << ")" << std::endl;
                std::cout << "  \t" << system->CTIFullPath() << std::endl;
                ++i;
            }

            // select CTI
            std::cout << std::endl << "Select CTI [0-" << deviceManager.Systems().size() - 1 << "]: ";
            std::cin >> i;

            if (std::cin.fail())
            {
                std::cout << "Invalid input! Using index 0." << std::endl;
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                i = 0;
            }

            if(i >= deviceManager.Systems().size())
            {
                std::cout << "Invalid index! Using index 0." << std::endl;
                i = 0;
            }

            cti = deviceManager.Systems().at(i)->CTIFullPath();

            // reset the deviceManager to clear out all found CTIs, so only the selected one will be used below
            deviceManager.Reset();
        }

        // add CTI to deviceManager
        std::cout << "Using CTI " << cti << std::endl << std::endl;
        deviceManager.AddProducerLibrary(cti);

        // update the deviceManager
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

        // list all available devices
        size_t i = 0;
        std::cout << "Devices available: " << std::endl;
        for (const auto& deviceDescriptor : deviceManager.Devices())
        {
            std::cout << i << ": " << deviceDescriptor->ModelName() << " ("
                      << deviceDescriptor->ParentInterface()->DisplayName() << ")" << std::endl;
            ++i;
        }

        // select a device to open
        size_t selectedDevice = 0;

        // select a device to open via user input or remove these lines to always open the first available device
        std::cout << std::endl << "Select device index to open [0-" << deviceManager.Devices().size() - 1 << "]: ";
        std::cin >> selectedDevice;

        if (std::cin.fail())
        {
            std::cout << "Invalid input! Using index 0." << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            selectedDevice = 0;
        }

        if (selectedDevice >= deviceManager.Devices().size())
        {
            std::cout << "Invalid index! Using index 0." << std::endl;
            selectedDevice = 0;
        }

        // open the selected device
        auto device =
            deviceManager.Devices().at(selectedDevice)->OpenDevice(peak::core::DeviceAccessType::Control);
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
                  << nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("WidthMax")->Value()
                  << " x "
                  << nodeMapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("HeightMax")->Value()
                  << std::endl;
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
