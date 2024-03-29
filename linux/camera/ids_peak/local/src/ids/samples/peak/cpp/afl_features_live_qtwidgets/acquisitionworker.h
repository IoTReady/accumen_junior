/*!
 * \file    acquisitionworker.h
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

#ifndef ACQUISITIONWORKER_H
#define ACQUISITIONWORKER_H

#include <peak_ipl/peak_ipl.hpp>
#include <peak/peak.hpp>

#include <peak_afl/peak_afl.hpp>

#include <QImage>
#include <QObject>


class AcquisitionWorker : public QObject
{
    Q_OBJECT

public:
    explicit AcquisitionWorker(QObject* parent = nullptr);
    ~AcquisitionWorker() = default;

    void Start();
    void Stop();
    void SetDataStream(std::shared_ptr<peak::core::DataStream> dataStream);
    void SetAutoFeatureManager(std::shared_ptr<peak::afl::Manager> autoFeatureManager);
    int GetImageWidth() const;
    int GetImageHeight() const;

private:
    std::shared_ptr<peak::core::DataStream> m_dataStream;
    std::shared_ptr<peak::core::NodeMap> m_nodemapRemoteDevice;
    std::shared_ptr<peak::afl::Manager> m_autoFeatureManager;

    bool m_running = false;

    unsigned int m_frameCounter = 0;
    unsigned int m_errorCounter = 0;

    size_t m_imageWidth = 0;
    size_t m_imageHeight = 0;

    std::unique_ptr<peak::ipl::ImageConverter> m_imageConverter;

signals:
    void ImageReceived(QImage image);
    void CounterChanged(unsigned int frameCounter, unsigned int errorCounter);
};

#endif // ACQUISITIONWORKER_H
