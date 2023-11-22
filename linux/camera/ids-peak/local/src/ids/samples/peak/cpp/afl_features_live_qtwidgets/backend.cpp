/*!
 * \file    backend.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-03-07
 * \since   1.0.0
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

#include "backend.h"

#include <peak_ipl/peak_ipl.hpp>

#include <peak/peak.hpp>

#include <peak_afl/peak_afl.hpp>

#include <QDebug>


BackEnd::BackEnd(QObject* parent)
    : QObject(parent)
    , m_acquisitionWorker(new AcquisitionWorker())
{
    // initialize peak library
    peak::Library::Initialize();

    // Worker thread that waits for new images from the camera
    m_acquisitionWorker->moveToThread(&m_acquisitionThread);

    // Worker must be started, when the acquisition starts, and deleted, when the worker thread finishes
    connect(&m_acquisitionThread, &QThread::started, m_acquisitionWorker, &AcquisitionWorker::Start);
    connect(&m_acquisitionThread, &QThread::finished, m_acquisitionWorker, &QObject::deleteLater);

    // Connect the signal from the worker thread when a new image was received with the corresponding
    // slot of the backend
    connect(m_acquisitionWorker, &AcquisitionWorker::ImageReceived, this, &BackEnd::ImageReceived);

    // Connect the signal from the worker thread when the counters have changed
    connect(m_acquisitionWorker, &AcquisitionWorker::CounterChanged, this, &BackEnd::CounterChanged);
}

BackEnd::~BackEnd()
{
    if (m_acquisitionWorker != nullptr)
    {
        m_acquisitionWorker->Stop();
        m_acquisitionThread.quit();
        m_acquisitionThread.wait();
    }

    // Deinitialize the AFL library
    try
    {
        peak::afl::library::Exit();
    }
    catch (const std::exception& e)
    {
        qDebug() << "Error occured when closing the peak::afl library:" << e.what();
    }

    CloseDevice();

    try
    {
        // close peak library
        peak::Library::Close();
    }
    catch (const std::exception& e)
    {
        qDebug() << "Error occured when closing the peak library:" << e.what();
    }
}

bool BackEnd::Start()
{
    if (!OpenDevice())
    {
        return false;
    }

    // Create AutoFeatureManager and AutoController before passing the AutoFeatureManager to the worker
    if (!InitializeAFL())
    {
        return false;
    }
    m_acquisitionWorker->SetAutoFeatureManager(m_autoFeatureManager);

    // Start thread execution
    m_acquisitionThread.start();

    return true;
}

bool BackEnd::OpenDevice()
{
    try
    {
        // Create instance of the device manager
        auto& deviceManager = peak::DeviceManager::Instance();

        // Update the device manager
        deviceManager.Update();

        // Return if no device was found
        if (deviceManager.Devices().empty())
        {
            emit MessageBoxTrigger("Error", "No device found");
            return false;
        }

        // open the first openable device in the device manager's device list
        size_t deviceCount = deviceManager.Devices().size();
        for (size_t i = 0; i < deviceCount; ++i)
        {
            if (deviceManager.Devices().at(i)->IsOpenable())
            {
                // Try to open all devices and check for available data streams
                m_device = deviceManager.Devices().at(i)->OpenDevice(peak::core::DeviceAccessType::Control);
                if (m_device->DataStreams().empty())
                {
                    m_device.reset();
                }
                else
                {
                    // stop after the first opened device with an available data stream
                    break;
                }
            }

            if (i == (deviceCount - 1))
            {
                emit MessageBoxTrigger("Error", "No device with available data streams could be opened");
                return false;
            }
        }

        if (m_device)
        {
            // Open standard data stream
            auto dataStreams = m_device->DataStreams();
            if (dataStreams.empty())
            {
                emit MessageBoxTrigger("Error", "Device has no DataStream");
                m_device.reset();
                return false;
            }

            try
            {
                // Open standard data stream
                m_dataStream = dataStreams.at(0)->OpenDataStream();
            }
            catch (const std::exception& e)
            {
                // Open data stream failed
                m_device.reset();
                emit MessageBoxTrigger("Error", QString("Failed to open DataStream\n") + e.what());
                return false;
            }

            // Get nodemap of remote device for all accesses to the genicam nodemap tree
            m_nodemapRemoteDevice = m_device->RemoteDevice()->NodeMaps().at(0);

            // To prepare for untriggered continuous image acquisition, load the default user set if available
            // and wait until execution is finished
            try
            {
                m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("UserSetSelector")
                    ->SetCurrentEntry("Default");

                m_nodemapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("UserSetLoad")->Execute();
                m_nodemapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("UserSetLoad")
                    ->WaitUntilDone();
            }
            catch (const peak::core::NotFoundException&)
            {
                // UserSet is not available
            }

            // Get the payload size for correct buffer allocation
            auto payloadSize = m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("PayloadSize")
                                   ->Value();

            // Get the minimum number of buffers that must be announced
            auto bufferCountMax = m_dataStream->NumBuffersAnnouncedMinRequired();

            // Allocate and announce image buffers and queue them
            for (size_t bufferCount = 0; bufferCount < bufferCountMax; ++bufferCount)
            {
                auto buffer = m_dataStream->AllocAndAnnounceBuffer(static_cast<size_t>(payloadSize), nullptr);
                m_dataStream->QueueBuffer(buffer);
            }

            // Configure worker
            m_acquisitionWorker->SetDataStream(m_dataStream);

            // If the camera has an internal autofocus or auto exposure mode,
            // it needs to be set to Off so the auto feature lib
            // can e.g. access the FocusStepper
            EnsureDeviceAutoFeaturesAreDisabled();

            return true;
        }
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Exception", e.what());
    }

    return false;
}

void BackEnd::CloseDevice()
{
    // if device was opened, try to stop acquisition
    if (m_device)
    {
        try
        {
            auto remoteNodeMap = m_device->RemoteDevice()->NodeMaps().at(0);
            remoteNodeMap->FindNode<peak::core::nodes::CommandNode>("AcquisitionStop")->Execute();
        }
        catch (const std::exception& e)
        {
            emit MessageBoxTrigger("Exception", e.what());
        }
    }

    // if data stream was opened, try to stop it and revoke its image buffers
    if (m_dataStream)
    {
        try
        {
            m_dataStream->KillWait();
            // In case we ran into an exception when e.g. initializing the AFL and the acquisition hasn't
            // been started yet
            if (m_dataStream->IsGrabbing())
            {
                m_dataStream->StopAcquisition(peak::core::AcquisitionStopMode::Default);
            }
            m_dataStream->Flush(peak::core::DataStreamFlushMode::DiscardAll);

            for (const auto& buffer : m_dataStream->AnnouncedBuffers())
            {
                m_dataStream->RevokeBuffer(buffer);
            }
        }
        catch (const std::exception& e)
        {
            emit MessageBoxTrigger("Exception", e.what());
        }
    }

    if (m_nodemapRemoteDevice)
    {
        try
        {
            // Unlock parameters after acquisition stop
            m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("TLParamsLocked")->SetValue(0);
        }
        catch (const std::exception& e)
        {
            emit MessageBoxTrigger("Exception", e.what());
        }
    }
}


bool BackEnd::InitializeAFL()
{
    // Initialize the peak::afl library
    try
    {
        peak::afl::library::Init();
    }
    catch (const peak::afl::error::Exception& e)
    {
        qDebug() << "Failed to initialize AFL lib:" << e.what();
        return false;
    }

    // Create an AutoFeatureManager for our NodeMap
    try
    {
        m_autoFeatureManager = std::make_shared<peak::afl::Manager>(peak::afl::Manager(m_nodemapRemoteDevice));
    }
    catch (const peak::afl::error::Exception& e)
    {
        qDebug() << "Could not create Auto Feature Manager:" << e.what();
        return false;
    }

    // Create and append a Controller to the manager
    // where the Controller is of type autofocus
    try
    {
        m_autoController = m_autoFeatureManager->CreateController(PEAK_AFL_CONTROLLER_TYPE_AUTOFOCUS);
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Autofocus Controller",
            "Could not create autofocus controller: " + QString::fromStdString(e.what())
                + "\n\nUnless there was an internal error, the camera probably does not support focus control!");
        return false;
    }

    UpdateSearchAlgorithms();
    UpdateSharpnessAlgorithms();

    m_aflInitialized = true;
    return true;
}


bool BackEnd::IsAutoFocusModeSupported()
{
    if (!m_aflInitialized || !m_autoController)
    {
        return false;
    }
    try
    {
        return m_autoController->IsModeSupported();
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Auto Mode Supported", "Exception: " + QString::fromStdString(e.what()));
        return false;
    }
}


peak::afl::Range<double> BackEnd::GetExposureRange()
{
    try
    {
        peak::afl::Range<double> result{};
        // Use the genericC++ api to query for the supported exposure range and increment (if there is one)
        result.min = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("ExposureTime")->Minimum();
        result.max = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("ExposureTime")->Maximum();
        if (m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("ExposureTime")
                ->HasConstantIncrement())
        {
            result.inc = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("ExposureTime")
                             ->Increment();
        }
        else
        {
            // set a reasonable default, which will be used by the gui slider
            result.inc = 1.0;
        }
        return result;
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger(
            "Get Exposure Range", "Failed to get exposure range: " + QString::fromStdString(e.what()));
        throw;
    }
}


double BackEnd::GetExposureInUs()
{
    try
    {
        double exposureInUs = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("ExposureTime")
                                  ->Value();
        return exposureInUs;
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Get Exposure", "Failed to get exposure: " + QString::fromStdString(e.what()));
        throw;
    }
}


bool BackEnd::SetGainSelectorToAll()
{
    // Check if the camera supports the gain setting
    if (!NodeIsAvailable("GainSelector"))
    {
        return false;
    }

    // Get a list of all available entries of GainSelector
    auto allEntries = m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("GainSelector")
                          ->Entries();
    std::string chosenEntry;
    // Make sure we select a mode for the GainSelector where the gain is applied to all channels
    for (const auto& entry : allEntries)
    {
        if ((entry->AccessStatus() != peak::core::nodes::NodeAccessStatus::NotAvailable)
            && (entry->AccessStatus() != peak::core::nodes::NodeAccessStatus::NotImplemented))
        {
            if (entry->StringValue() == "DigitalAll")
            {
                chosenEntry = entry->SymbolicValue();
                // prefer AnalogAll -> don't stop here, so if AnalogAll is available it will overwrite chosenEntry
            }
            else if ((entry->StringValue() == "AnalogAll") || (entry->StringValue() == "All"))
            {
                chosenEntry = entry->SymbolicValue();
                break;
            }
        }
    }

    if (chosenEntry.empty())
    {
        // no appropriate GainSelector setting found
        return false;
    }

    try
    {
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("GainSelector")
            ->SetCurrentEntry(chosenEntry);
        return true;
    }
    catch (std::exception const& e)
    {
        emit MessageBoxTrigger("Analog Gain Selector", QString::fromStdString(e.what()));
    }

    return false;
}

peak::afl::Range<double> BackEnd::GetGainRange()
{
    try
    {
        peak::afl::Range<double> result{};
        result.max = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("Gain")->Maximum();
        result.min = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("Gain")->Minimum();
        const double DEFAULT_GAIN_INCREMENT = 0.01;
        result.inc = DEFAULT_GAIN_INCREMENT;
        if (m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("Gain")->HasConstantIncrement())
        {
            result.inc = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("Gain")->Increment();
        }

        return result;
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Get Gain Range", "Failed to get gain range: " + QString::fromStdString(e.what()));
        throw;
    }
}

double BackEnd::GetGain()
{
    try
    {
        double gain = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("Gain")->Value();
        return gain;
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Get Gain", "Failed to get gain: " + QString::fromStdString(e.what()));
        throw;
    }
}


peak::afl::Range<double> BackEnd::GetFrameRateRange()
{
    try
    {
        peak::afl::Range<double> result{};
        result.min = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
                         ->Minimum();
        result.max = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
                         ->Maximum();
        if (m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
                ->HasConstantIncrement())
        {
            result.inc = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
                             ->Increment();
        }
        else
        {
            // If there is no increment, choose a suitable increment for a GUI control element (e.g. a slider)
            const double DEFAULT_FRAME_RATE_INCREMENT = 0.1;
            result.inc = DEFAULT_FRAME_RATE_INCREMENT;
        }

        return result;
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger(
            "Get Frame Rate Range", "Failed to get frame rate range: " + QString::fromStdString(e.what()));
        throw;
    }
}


double BackEnd::GetFrameRate()
{
    try
    {
        double frameRate =
            m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")->Value();
        return frameRate;
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Get Frame Rate", "Failed to get frame rate: " + QString::fromStdString(e.what()));
        throw;
    }
}


void BackEnd::UpdateSearchAlgorithms()
{
    try
    {
        m_searchAlgorithms = m_autoController->GetAlgorithmList();
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Update search algorithms", QString::fromStdString(e.what()));
    }
}

const std::vector<peak_afl_controller_algorithm>& BackEnd::GetSearchAlgorithms()
{
    return m_searchAlgorithms;
}


void BackEnd::UpdateSharpnessAlgorithms()
{
    try
    {
        m_sharpnessAlgorithms = m_autoController->GetSharpnessAlgorithmList();
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Update sharpness algorithms", QString::fromStdString(e.what()));
    }
}


const std::vector<peak_afl_controller_sharpness_algorithm>& BackEnd::GetSharpnessAlgorithms()
{
    return m_sharpnessAlgorithms;
}


bool BackEnd::IsHysteresisSupported()
{
    try
    {
        return m_autoController->IsHysteresisSupported();
    }
    catch (const peak::afl::error::Exception& e)
    {
        qDebug() << "Exception occured when checking for hysteresis support:" << e.what();
        return false;
    }
}


peak::afl::Range<uint8_t> BackEnd::GetHysteresisRange()
{
    try
    {
        return m_autoController->GetHysteresisRange();
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Get Hysteresis Range", QString::fromStdString(e.what()));
        return peak::afl::Range<uint8_t>{};
    }
}


uint8_t BackEnd::GetHysteresis()
{
    try
    {
        return m_autoController->GetHysteresis();
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Get Hysteresis", QString::fromStdString(e.what()));
        return 0;
    }
}


bool BackEnd::IsSearchRangeSupported()
{
    try
    {
        return m_autoController->IsLimitSupported();
    }
    catch (const peak::afl::error::Exception&)
    {
        return false;
    }
}


peak_afl_controller_limit BackEnd::GetSearchRangeLimit()
{
    try
    {
        return m_autoController->GetLimit();
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Search Range Limit", QString::fromStdString(e.what()));
        return peak_afl_controller_limit{ -1, -1 };
    }
}


peak::afl::Range<int64_t> BackEnd::GetSearchRangeRange()
{
    try
    {
        peak::afl::Range<int64_t> result{};
        // Before accessing FocusStepper, make sure OpticControllerSelector is set correctly
        // Set OpticControllerSelector to "OpticController0"
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("OpticControllerSelector")
            ->SetCurrentEntry("OpticController0");
        // Determine the current FocusStepper
        result.min = m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("FocusStepper")
                         ->Minimum();
        result.max = m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("FocusStepper")
                         ->Maximum();
        result.inc = m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("FocusStepper")
                         ->Increment();
        return result;
    }
    catch (const peak::core::Exception& e)
    {
        emit MessageBoxTrigger("Search Limit Range", QString::fromStdString(e.what()));
        throw;
    }
}


bool BackEnd::IsAutoFocusROISupported()
{
    // NOTE: using the weighted ROI system here, since that is the only one that's
    // supported by the autofocus controller
    // Using one weighted ROI with a strong weight will have the same effect though!
    try
    {
        return m_autoController->IsWeightedROISupported();
    }
    catch (const peak::afl::error::Exception&)
    {
        return false;
    }
}


peak_afl_rectangle BackEnd::GetROI()
{
    auto rois = m_autoController->GetWeightedROIs();
    // NOTE: Assuming there is a default ROI if ROI is supported
    peak_afl_rectangle roi = rois.at(0).roi;
    return roi;
}


int BackEnd::GetImageWidth() const
{
    return m_acquisitionWorker->GetImageWidth();
}


int BackEnd::GetImageHeight() const
{
    return m_acquisitionWorker->GetImageHeight();
}


peak_afl_size BackEnd::GetROIMinSize()
{
    // Query minimum supported size for the ROI that the autofocus can use
    try
    {
        return m_autoController->GetWeightedROIMinSize();
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("ROI Get Minimum Size", QString::fromStdString(e.what()));
        return peak_afl_size{ 0, 0 };
    }
}


bool BackEnd::SetFocusMode(peak_afl_controller_automode mode)
{
    bool success = false;
    if (m_autoController != nullptr && m_autoFeatureManager != nullptr)
    {
        try
        {
            // NOTE: When setting the focus mode to "Once" it will be reset to "Off" after
            // the autofocus is done adjusting
            m_autoController->SetMode(mode);
            success = true;
        }
        catch (const peak::afl::error::Exception& e)
        {
            qDebug() << "Could not set AutoController mode:" << e.what();
        }
    }
    else
    {
        qDebug("No auto manager or controller set!");
    }

    return success;
}


void BackEnd::SetExposure(double exposureTimeInUs, double& fpsOut, double& maxFpsOut)
{
    try
    {
        // Set ExposureTime in us
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("ExposureTime")
            ->SetValue(exposureTimeInUs);
        // Update frame rate in case the exposure time was too high to keep up with the current setting
        fpsOut = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
                     ->Value();
        // Query for the maximum frame rate depending on the newly set exposure time
        // NOTE: the formula would be (with an additional cut-off point depending on the camera model)
        // Calculate the maximum frame rate that can be achieved using the new exposureTime
        // 1s in us = 1000ms * 1000
        // double maxFPS = 1000.0f * 1000.0f / static_cast<double>(exposureTimeInUs);
        maxFpsOut = m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
                        ->Maximum();
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Set Exposure", "Failed to set exposure: " + QString::fromStdString(e.what()));
    }
}


void BackEnd::SetGain(double gain)
{
    try
    {
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("Gain")->SetValue(gain);
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Set Gain", "Failed to set gain: " + QString::fromStdString(e.what()));
    }
}


void BackEnd::SetFrameRate(double frameRate)
{
    try
    {
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")
            ->SetValue(frameRate);
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger("Set FrameRate", "Failed to set frame rate: " + QString::fromStdString(e.what()));
    }
}


void BackEnd::SetSearchAlgorithm(int searchAlgorithmIndex)
{
    try
    {
        const auto algo = m_searchAlgorithms.at(searchAlgorithmIndex);
        m_autoController->SetAlgorithm(algo);
    }
    catch (const std::out_of_range&)
    {
        emit MessageBoxTrigger("Set Search Algorithm", "Search algorithm index out of bounds!");
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Set Search Algorithm", QString::fromStdString(e.what()));
    }
}


void BackEnd::SetSharpnessAlgorithm(int sharpnessAlgorithmIndex)
{
    try
    {
        const auto algo = m_sharpnessAlgorithms.at(sharpnessAlgorithmIndex);
        m_autoController->SetSharpnessAlgorithm(algo);
    }
    catch (const std::out_of_range&)
    {
        emit MessageBoxTrigger("Set Sharpness Algorithm Set", "Sharpness algorithm index out of bounds!");
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Set Sharpness Algorithm", QString::fromStdString(e.what()));
    }
}


void BackEnd::SetHysteresis(uint8_t hysteresis)
{
    try
    {
        m_autoController->SetHysteresis(hysteresis);
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Hysteresis", QString::fromStdString(e.what()));
    }
}


bool BackEnd::SetSearchRange(int min, int max)
{
    try
    {
        peak_afl_controller_limit limit{ min, max };
        m_autoController->SetLimit(limit);
        return true;
    }
    catch (const peak::afl::error::Exception& e)
    {
        emit MessageBoxTrigger("Search range", QString::fromStdString(e.what()));
        return false;
    }
}


bool BackEnd::SetROI(peak_afl_rectangle roi)
{
    bool success = false;
    auto imageWidth = static_cast<uint32_t>(GetImageWidth());
    auto imageHeight = static_cast<uint32_t>(GetImageHeight());
    if (((roi.x + roi.width) <= imageWidth) && ((roi.y + roi.height) <= imageHeight))
    {
        // ROI in bounds!
        peak_afl_weighted_rectangle weighted;
        weighted.roi = roi;
        // fixed strong weight, since we use the weighted/multiple ROI api with only a single ROI
        weighted.weight = PEAK_AFL_CONTROLLER_ROI_WEIGHT_STRONG;
        try
        {
            m_autoController->SetWeightedROI(weighted);
            success = true;
        }
        catch (const peak::afl::error::Exception& e)
        {
            emit MessageBoxTrigger("Set Autofocus ROI", QString::fromStdString(e.what()));
        }
    }
    else
    {
        qDebug() << "ROI out of bounds!";
    }

    return success;
}


bool BackEnd::SetAutoControllerFinishedCallback(const peak::afl::callback::FinishedCallback& callback)
{
    try
    {
        m_autoController->RegisterFinishedCallback(callback);
        return true;
    }
    catch (const peak::afl::error::Exception& e)
    {
        qDebug() << "Failed to set controller callback:" << QString::fromStdString(e.what());
        return false;
    }
}

bool BackEnd::SetAutoControllerProcessingCallback(const peak::afl::callback::DataProcessingCallback& callback)
{
    try
    {
        m_autoController->RegisterDataProcessingCallback(callback);
        return true;
    }
    catch (const peak::afl::error::Exception& e)
    {
        qDebug() << "Failed to set controller callback:" << QString::fromStdString(e.what());
        return false;
    }
}


bool BackEnd::HasDeviceAutoFocus()
{
    // Check if the device has an internal autofocus
    return NodeIsAvailable("FocusAuto");
}


void BackEnd::EnsureDeviceAutoFeaturesAreDisabled()
{
    try
    {
        if (HasDeviceAutoFocus())
        {
            m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("FocusAuto")
                ->SetCurrentEntry("Off");
        }
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger(
            "Internal Autofocus", "Failed to disable internal autofocus: " + QString::fromStdString(e.what()));
    }

    try
    {
        // ExposureAuto, GainAuto, etc. only changeable if SequencerMode == Off
        if (NodeIsWriteable("SequencerMode"))
        {
            m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("SequencerMode")
                ->SetCurrentEntry("Off");
        }
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger(
            "Sequencer Mode", "Failed to change Sequencer Mode: " + QString::fromStdString(e.what()));
    }

    try
    {
        if (NodeIsWriteable("ExposureAuto"))
        {
            m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("ExposureAuto")
                ->SetCurrentEntry("Off");
        }
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger(
            "Exposure Auto", "Failed to disable internal auto exposure: " + QString::fromStdString(e.what()));
    }

    try
    {
        if (NodeIsWriteable("GainAuto"))
        {
            m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("GainAuto")
                ->SetCurrentEntry("Off");
        }
    }
    catch (const std::exception& e)
    {
        emit MessageBoxTrigger(
            "Gain Auto", "Failed to disable internal auto gain: " + QString::fromStdString(e.what()));
    }
}

bool BackEnd::NodeIsAvailable(const std::string& node_name)
{
    if (m_nodemapRemoteDevice == nullptr)
    {
        return false;
    }

    if (m_nodemapRemoteDevice->HasNode(node_name))
    {
        auto node = m_nodemapRemoteDevice->FindNode(node_name);
        if (node->AccessStatus() != peak::core::nodes::NodeAccessStatus::NotImplemented
            && node->AccessStatus() != peak::core::nodes::NodeAccessStatus::NotAvailable)
        {
            return true;
        }
    }

    return false;
}

bool BackEnd::NodeIsReadable(const std::string& node_name)
{
    if (m_nodemapRemoteDevice == nullptr)
    {
        return false;
    }

    if (m_nodemapRemoteDevice->HasNode(node_name))
    {
        auto node = m_nodemapRemoteDevice->FindNode(node_name);
        if (node->AccessStatus() == peak::core::nodes::NodeAccessStatus::ReadOnly
            || node->AccessStatus() == peak::core::nodes::NodeAccessStatus::ReadWrite)
        {
            return true;
        }
    }

    return false;
}

bool BackEnd::NodeIsWriteable(const std::string& node_name)
{
    if (m_nodemapRemoteDevice == nullptr)
    {
        return false;
    }

    if (m_nodemapRemoteDevice->HasNode(node_name))
    {
        auto node = m_nodemapRemoteDevice->FindNode(node_name);
        if (node->AccessStatus() == peak::core::nodes::NodeAccessStatus::WriteOnly
            || node->AccessStatus() == peak::core::nodes::NodeAccessStatus::ReadWrite)
        {
            return true;
        }
    }

    return false;
}
