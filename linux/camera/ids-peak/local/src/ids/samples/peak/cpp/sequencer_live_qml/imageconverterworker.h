/*!
 * \file    imageconverterworker.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   1.1.6
 *
 * \brief   The ImageConverter class is used in a worker thread to convert
 *          buffers received from the datastream to images that can be displayed.
 *
 * \version 1.1.0
 *
 * Copyright (C) 2020 - 2023, IDS Imaging Development Systems GmbH.
 *
 * The information in this document is subject to change without
 * notice and should not be construed as a commitment by IDS Imaging Development Systems GmbH.
 * IDS Imaging Development Systems GmbH does not assume any responsibility
 * for any errors that may appear in this document.
 *
 * This document, or source code, is provided solely as an example
 * of how to utilize IDS Imaging Development Systems GmbH software libraries in a sample application.
 * IDS Imaging Development Systems GmbH does not assume any responsibility
 * for the use or reliability of any portion of this document.
 *
 * General permission to copy or modify is hereby granted.
 */

#ifndef IMAGECONVERTERWORKER_H
#define IMAGECONVERTERWORKER_H

#include <peak/peak.hpp>
#include <peak_ipl/peak_ipl.hpp>

#include <QImage>
#include <QObject>

class ImageConverterWorker : public QObject
{
    Q_OBJECT
public:
    ImageConverterWorker();
    ~ImageConverterWorker() override = default;

    void setDataStream(const std::shared_ptr<peak::core::DataStream>& dataStream);
    void setImageCount(unsigned int imageCount);

public slots:
    void convert(const std::shared_ptr<peak::core::Buffer>& buffer);
    void resetCounter();

private:
    std::shared_ptr<peak::core::DataStream> m_dataStream;
    std::shared_ptr<peak::core::NodeMap> m_nodemapRemoteDevice;

    unsigned int m_converterCounter = 0;
    unsigned int m_imageCount = 1;
    unsigned long long m_timestamp_previous_us = 0;

    size_t m_imageWidth = 0;
    size_t m_imageHeight = 0;

    std::unique_ptr<peak::ipl::ImageConverter> m_imageConverter;

signals:
    void imageReceived(
        QImage image, unsigned int iterator, unsigned long long timestamp, unsigned long long timestampDelta);
    void counterChanged(const unsigned int);
};

#endif // IMAGECONVERTERWORKER_H
