/*!
 * \file    acquisitionworker.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-03-07
 * \since   1.0.0
 *
 * \brief   The AcquisitionWorker class is used in a worker thread to capture
 *          images from the device continuously and do an image conversion into
 *          a desired pixel format.
 *
 * \version 1.2.0
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

#include "acquisitionworker.h"

#include <peak/converters/peak_buffer_converter_ipl.hpp>

#include <QDebug>

#include <cmath>
#include <cstring>
#include <memory>

#define USE_IMAGE_CONVERTER


AcquisitionWorker::AcquisitionWorker(QObject* parent)
    : QObject(parent)
{
    m_imageConverter = std::make_unique<peak::ipl::ImageConverter>();
}

void AcquisitionWorker::Start()
{
    try
    {
        // Lock critical features to prevent them from changing during acquisition
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("TLParamsLocked")->SetValue(1);

        // Determine image size
        m_imageWidth = m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("Width")->Value();
        m_imageHeight = m_nodemapRemoteDevice->FindNode<peak::core::nodes::IntegerNode>("Height")->Value();

        // Pre-allocate images for conversion that can be used simultaneously
        // This is not mandatory but it can increase the speed of image conversions
        size_t imageCount = 1;
        const auto inputPixelFormat = static_cast<peak::ipl::PixelFormatName>(
            m_nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("PixelFormat")
                ->CurrentEntry()
                ->Value());

        m_imageConverter->PreAllocateConversion(
            inputPixelFormat, peak::ipl::PixelFormatName::BGRa8, m_imageWidth, m_imageHeight, imageCount);

        // Start acquisition
        m_dataStream->StartAcquisition();
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStart")->Execute();
        m_nodemapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("AcquisitionStart")->WaitUntilDone();
    }
    catch (const std::exception& e)
    {
        qDebug() << "Exception: " << e.what();
    }

    m_running = true;

    while (m_running)
    {
        try
        {
            // Get buffer from device's datastream
            const auto buffer = m_dataStream->WaitForFinishedBuffer(5000);

            QImage qImage(static_cast<int>(m_imageWidth), static_cast<int>(m_imageHeight), QImage::Format_RGB32);

            // Process IDS peak IPL image in the AutoFeatureManager to apply all AutoController operations: e.g. software auto
            // focus etc.
            auto img = peak::BufferTo<peak::ipl::Image>(buffer);
            m_autoFeatureManager->Process(img);

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
            const auto imageByteSize = static_cast<size_t>(qImage.byteCount());
#else
            const auto imageByteSize = static_cast<size_t>(qImage.sizeInBytes());
#endif
            // Create IDS peak IPL image for debayering and convert it to BGRa8 format
#ifdef USE_IMAGE_CONVERTER
            // Using the image converter ...
            m_imageConverter->Convert(peak::BufferTo<peak::ipl::Image>(buffer),
                peak::ipl::PixelFormatName::BGRa8, qImage.bits(), imageByteSize);
#else
            // ... or without image converter
            peak::BufferTo<peak::ipl::Image>(buffer).ConvertTo(
                peak::ipl::PixelFormatName::BGRa8, qImage.bits(), imageByteSize);
#endif


            // Queue buffer so that it can be used again
            m_dataStream->QueueBuffer(buffer);

            // Emit signal that the image is ready to be displayed
            emit ImageReceived(qImage);

            m_frameCounter++;
        }
        catch (const std::exception& e)
        {
            m_errorCounter++;

            qDebug() << "Exception: " << e.what();
        }

        // Send signal with current frame and error counter
        emit CounterChanged(m_frameCounter, m_errorCounter);
    }
}

void AcquisitionWorker::Stop()
{
    m_running = false;
}

void AcquisitionWorker::SetDataStream(std::shared_ptr<peak::core::DataStream> dataStream)
{
    m_dataStream = std::move(dataStream);
    m_nodemapRemoteDevice = m_dataStream->ParentDevice()->RemoteDevice()->NodeMaps().at(0);
}

void AcquisitionWorker::SetAutoFeatureManager(std::shared_ptr<peak::afl::Manager> autoFeatureManager)
{
    m_autoFeatureManager = std::move(autoFeatureManager);
}

int AcquisitionWorker::GetImageHeight() const
{
    return static_cast<int>(m_imageHeight);
}

int AcquisitionWorker::GetImageWidth() const
{
    return static_cast<int>(m_imageWidth);
}
