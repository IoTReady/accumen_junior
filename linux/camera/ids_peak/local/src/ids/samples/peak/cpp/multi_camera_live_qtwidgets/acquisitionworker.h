/*!
 * \file    acquisitionworker.h
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

#ifndef ACQUISITIONWORKER_H
#define ACQUISITIONWORKER_H

#include "displaywindow.h"

#include <peak/peak.hpp>
#include <peak_ipl/peak_ipl.hpp>

#include <QImage>
#include <QLabel>
#include <QObject>

#include <cstdint>


class MainWindow;


class AcquisitionWorker : public QObject
{
    Q_OBJECT

public:
    AcquisitionWorker(MainWindow* parent, DisplayWindow* displayWindow,
        std::shared_ptr<peak::core::DataStream> dataStream, peak::ipl::PixelFormatName pixelFormat,
        size_t imageWidth, size_t imageHeight);
    ~AcquisitionWorker() = default;

    void Stop();

public slots:
    void Start();

private:
    MainWindow* m_parent;
    DisplayWindow* m_displayWindow;

    std::shared_ptr<peak::core::DataStream> m_dataStream;
    peak::ipl::PixelFormatName m_outputPixelFormat;

    bool m_running;
    bool m_customNodesAvailable;

    unsigned int m_frameCounter;
    unsigned int m_errorCounter;

    size_t m_imageWidth;
    size_t m_imageHeight;

    std::unique_ptr<peak::ipl::ImageConverter> m_imageConverter;

signals:
    void ImageReceived(QImage image);
    void UpdateCounters(double frameTime_ms, double conversionTime_ms, unsigned int frameCounter,
        unsigned int errorCounter, int incomplete, int dropped, int lost, bool showCustomNodes);
};

#endif // ACQUISITIONWORKER_H
