/*!
 * \file    acquisitionworker.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   1.1.6
 *
 * \brief   The AcquisitionWorker class is used in a worker thread to capture
 *          images from the device continuously and do an image conversion into
 *          a desired pixel format.
 *
 * \version 1.2.0
 *
 * Copyright (C) 2020 - 2023, IDS Imaging Development Systems GmbH.
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

#include "chronometer.h"
#include "mainwindow.h"

#include <QImage>


AcquisitionWorker::AcquisitionWorker(MainWindow* parent, DisplayWindow* displayWindow,
    std::shared_ptr<peak::core::DataStream> dataStream, peak::ipl::PixelFormatName pixelFormat,
    size_t imageWidth, size_t imageHeight)
{
    m_parent = parent;
    m_displayWindow = displayWindow;
    m_dataStream = dataStream;
    m_outputPixelFormat = pixelFormat;
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_frameCounter = 0;
    m_errorCounter = 0;

    m_imageConverter = std::make_unique<peak::ipl::ImageConverter>();

    auto isNodeReadable = [this](std::string const& nodeName) {
        if (!m_dataStream->NodeMaps().at(0)->HasNode(nodeName))
        {
            return false;
        }

        return (m_dataStream->NodeMaps().at(0)->FindNode(nodeName)->AccessStatus()
                   == peak::core::nodes::NodeAccessStatus::ReadOnly)
            || (m_dataStream->NodeMaps().at(0)->FindNode(nodeName)->AccessStatus()
                == peak::core::nodes::NodeAccessStatus::ReadWrite);
    };

    m_customNodesAvailable = isNodeReadable("StreamIncompleteFrameCount")
        && isNodeReadable("StreamDroppedFrameCount") && isNodeReadable("StreamLostFrameCount");
}

void AcquisitionWorker::Start()
{
    QImage::Format qImageFormat;

    auto bytesPerPixel = size_t{ 0 };

    switch (m_outputPixelFormat)
    {
    case peak::ipl::PixelFormatName::BGRa8: 
    {
        qImageFormat = QImage::Format_RGB32;
        bytesPerPixel = 4;
        break;
    }
    case peak::ipl::PixelFormatName::RGB8: 
    {
        qImageFormat = QImage::Format_RGB888;
        bytesPerPixel = 3;
        break;
    }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    case peak::ipl::PixelFormatName::RGB10p32: {
        qImageFormat = QImage::Format_BGR30;
        bytesPerPixel = 4;
        break;
    }
#endif
    case peak::ipl::PixelFormatName::BayerRG8: 
    case peak::ipl::PixelFormatName::Mono8: 
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
        qImageFormat = QImage::Format_Grayscale8;
#else
        qImageFormat = QImage::Format_Indexed8;
#endif
        bytesPerPixel = 1;
        break;
    }
    default: 
    {
        qImageFormat = QImage::Format_RGB32;
        bytesPerPixel = 4;
    }
    }

    // Pre-allocate images for conversion that can be used simultaneously
    // This is not mandatory but it can increase the speed of image conversions
    size_t imageCount = 1;

    auto nodemapRemoteDevice = m_dataStream->ParentDevice()->RemoteDevice()->NodeMaps().at(0);

    try
    {
        const auto inputPixelFormat = static_cast<peak::ipl::PixelFormatName>(
            nodemapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("PixelFormat")
                ->CurrentEntry()
                ->Value());

        m_imageConverter->PreAllocateConversion(
            inputPixelFormat, m_outputPixelFormat, m_imageWidth, m_imageHeight, imageCount);
    }
    catch (const std::exception& e)
    {
        qDebug() << "Exception: " << e.what();
    }

    int incomplete = 0;
    int dropped = 0;
    int lost = 0;

    Chronometer chronometerConversion;
    Chronometer chronometerFrameTime;

    double frameTime_ms = 0;
    double conversionTime_ms = 0;

    m_running = true;

    while (m_running)
    {
        try
        {
            // Wait 5 seconds for an image from the camera
            auto buffer = m_dataStream->WaitForFinishedBuffer(5000);

            chronometerConversion.Start();

            QImage qImage(static_cast<int>(m_imageWidth), static_cast<int>(m_imageHeight), qImageFormat);

            // Create IDS peak IPL image for debayering and convert it to output pixel format

            // Using the image converter ...
            m_imageConverter->Convert(peak::BufferTo<peak::ipl::Image>(buffer), m_outputPixelFormat,
                qImage.bits(), static_cast<size_t>(qImage.byteCount()));

            // ... or without image converter
            // peak::BufferTo<peak::ipl::Image>(buffer).ConvertTo(
            //     outputPixelFormat, qImage.bits(), static_cast<size_t>(qImage.byteCount()));

            conversionTime_ms = chronometerConversion.GetTimeSinceStart_ms();

            // Requeue buffer
            m_dataStream->QueueBuffer(buffer);

            // Send signal to update the display
            emit ImageReceived(qImage);

            frameTime_ms = chronometerFrameTime.GetTimeSinceStart_ms();
            chronometerFrameTime.Start();

            m_frameCounter++;

            if (m_customNodesAvailable)
            {
                // Missing packets on the interface, event after 1 resend
                incomplete = static_cast<int>(
                    m_dataStream->NodeMaps()
                        .at(0)
                        ->FindNode<peak::core::nodes::IntegerNode>("StreamIncompleteFrameCount")
                        ->Value());

                // Camera buffer overrun (sensor data too fast for interface)
                dropped = static_cast<int>(
                    m_dataStream->NodeMaps()
                        .at(0)
                        ->FindNode<peak::core::nodes::IntegerNode>("StreamDroppedFrameCount")
                        ->Value());

                // User buffer overrun (application too slow to process the camera data)
                lost = static_cast<int>(m_dataStream->NodeMaps()
                                            .at(0)
                                            ->FindNode<peak::core::nodes::IntegerNode>("StreamLostFrameCount")
                                            ->Value());
            }
        }
        catch (const std::exception&)
        {
            // Without a sleep the GUI will be blocked completely and the CPU load will rise!
            QThread::msleep(1000);

            m_errorCounter++;
        }

        emit UpdateCounters(frameTime_ms, conversionTime_ms, m_frameCounter, m_errorCounter, incomplete, dropped, lost,
            m_customNodesAvailable);
    }
}

void AcquisitionWorker::Stop()
{
    m_running = false;
}
