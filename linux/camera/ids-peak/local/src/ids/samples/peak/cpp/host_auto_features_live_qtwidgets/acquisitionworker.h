/*!
 * \file    acquisitionworker.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   1.2.0
 *
 * \brief   The AcquisitionWorker class is used in a worker thread to capture
 *          images from the device continuously and do an image conversion into
 *          a desired pixel format.
 *
 * \version 1.2.0
 *
 * Copyright (C) 2021 - 2023, IDS Imaging Development Systems GmbH.
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

#ifndef ACQUISITIONWORKER_H
#define ACQUISITIONWORKER_H

#include <peak_ipl/peak_ipl.hpp>
#include <peak/peak.hpp>


#include <QImage>
#include <QObject>


class AcquisitionWorker : public QObject
{
    Q_OBJECT

public:
    AcquisitionWorker(QObject* parent = nullptr);
    ~AcquisitionWorker() = default;

    void Start();
    void Stop();

    void SetDataStream(std::shared_ptr<peak::core::DataStream> dataStream);

private:
    std::shared_ptr<peak::core::DataStream> m_dataStream;
    std::shared_ptr<peak::core::NodeMap> m_nodemapRemoteDevice;

    bool m_running = false;

    unsigned int m_frameCounter = 0;
    unsigned int m_errorCounter = 0;

    size_t m_imageWidth = 0;
    size_t m_imageHeight = 0;
    size_t m_bufferWidth = 0;
    size_t m_bufferHeight = 0;

    std::unique_ptr<peak::ipl::ImageConverter> m_imageConverter;

signals:
    void imageReceived(QImage image);
    void imageReceived(const peak::ipl::Image* image);
    void counterChanged(unsigned int frameCounter, unsigned int errorCounter);
};

#endif // ACQUISITIONWORKER_H
