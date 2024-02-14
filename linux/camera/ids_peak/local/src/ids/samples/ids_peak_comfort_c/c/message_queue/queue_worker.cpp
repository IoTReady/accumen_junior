/*!
 * \file    queue_worker.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-04-24
 * \since   2.5.0
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

#include "queue_worker.h"
#include "backend.h"

#include <QDebug>

void QueueWorker::run()
{
    char data[128]{};

    handle = backend_messagequeue_prepare();
    if (handle == PEAK_INVALID_HANDLE)
    {
        return;
    }

    while (doRun.load())
    {
        if (doPause.load())
        {
            QThread::msleep(200);
            continue;
        }

        auto hMessage = backend_messagequeue_get_message(handle);
        if (!doRun.load())
        {
            break;
        }

        if (doPause.load() || hMessage == PEAK_INVALID_HANDLE)
        {
            continue;
        }

        peak_message_info info{};
        auto status = backend_message_info(hMessage, &info);
        if (status != PEAK_STATUS_SUCCESS)
        {
            backend_message_release(hMessage);
            continue;
        }

        if (backend_message_data_string(hMessage, data, sizeof(data)) == PEAK_STATUS_SUCCESS)
        {
            if (info.hCam == PEAK_INVALID_HANDLE)
            {
                newMessage(MessageData{ info.hostMessageTimestamp_ns, info.messageID, "-", info.type, QString(data) });
            }
            else
            {
                auto id = backend_camera_id_from_handle(info.hCam);
                newMessage(MessageData{
                    info.hostMessageTimestamp_ns, info.messageID, QString("Camera %1").arg(id), info.type, QString(data) });
            }
        }
        backend_message_release(hMessage);
    }

    if (handle)
    {
        backend_messagequeue_cleanup(handle);
        handle = nullptr;
    }
}

std::string Queue::messageTypeToStr(peak_message_type type)
{
    static std::unordered_map<peak_message_type, std::string> messageTypes{
        // Device Types
        { PEAK_MESSAGE_TYPE_INVALID, "Invalid" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_CRITICAL_ERROR, "RemoteDevice Critical Error" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_ERROR, "RemoteDevice Error" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_EVENT_DROPPED, "RemoteDevice Event Dropped" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_EXPOSURE_START, "RemoteDevice Exposure Start" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_EXPOSURE_END, "RemoteDevice Exposure End" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_FRAME_START, "RemoteDevice Frame Start" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_FRAME_DROPPED, "RemoteDevice Frame Dropped" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_MISSED_TRIGGER_EXPOSURE, "RemoteDevice Missed Trigger Exposure" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_MISSED_TRIGGER_LINE, "RemoteDevice Missed Trigger Line" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_PTP_MASTER_SYNC_LOST, "RemoteDevice Ptp Master Sync Lost" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_TEMPERATURE, "RemoteDevice Temperature" },
        { PEAK_MESSAGE_TYPE_REMOTE_DEVICE_TEST, "RemoteDevice Test" },

        // AutoFeature Types
        { PEAK_MESSAGE_TYPE_AUTO_FOCUS_ONCE_FINISHED, "AutoFocus Once Finished" },
        { PEAK_MESSAGE_TYPE_AUTO_FOCUS_NEW_DATA, "AutoFocus New Data" },
        { PEAK_MESSAGE_TYPE_AUTO_WHITEBALANCE_ONCE_FINISHED, "AutoWhitebalance Once Finished" },
        { PEAK_MESSAGE_TYPE_AUTO_BRIGHTNESS_EXPOSURE_ONCE_FINISHED, "AutoBrightness Exposure Once Finished" },
        { PEAK_MESSAGE_TYPE_AUTO_BRIGHTNESS_GAIN_ONCE_FINISHED, "AutoBrightness Gain Once Finished" },
        { PEAK_MESSAGE_TYPE_AUTO_BRIGHTNESS_ONCE_FINISHED, "AutoBrightness Once Finished" },
    };

    auto it = std::find_if(
        messageTypes.cbegin(), messageTypes.cend(), [type](const auto& el) { return el.first == type; });

    if (it != messageTypes.cend())
    {
        return it->second;
    }

    return "Invalid";
}

Queue::Queue(QObject* parent)
    : QObject(parent)
{
    m_worker = new QueueWorker();

    connect(m_worker, &QueueWorker::newMessage, this, &Queue::newMessage);

    connect(m_worker, &QThread::finished, m_worker, &QObject::deleteLater);
    m_worker->start();
}

Queue::~Queue()
{
    stop();
}

void Queue::disableMessage(void* hCam, int type)
{
    // we need to stop first to disable the type and start it again after
    pause();
    backend_messagequeue_message_disable(
        m_worker->handle, static_cast<peak_camera_handle>(hCam), static_cast<peak_message_type>(type));
    resume();
}

void Queue::enableMessage(void* hCam, int type)
{
    // we need to stop first to disable the type and start it again after
    pause();
    backend_messagequeue_message_enable(
        m_worker->handle, static_cast<peak_camera_handle>(hCam), static_cast<peak_message_type>(type));
    resume();
}

bool Queue::isMessageSupported(void* hCam, int type)
{
    return backend_messagequeue_message_supported(
        m_worker->handle, static_cast<peak_camera_handle>(hCam), static_cast<peak_message_type>(type));
}

void Queue::stop()
{
    if (m_worker && m_worker->doRun)
    {
        m_worker->doRun = false;
        peak_message_queue_handle handle = PEAK_INVALID_HANDLE;

        std::swap(handle, m_worker->handle);

        backend_messagequeue_cleanup(handle);
        m_worker->wait();
    }
}

void Queue::pause()
{
    if (!m_worker->doPause.load())
    {
        m_worker->doPause = true;
        backend_message_queue_stop(m_worker->handle);
    }
}

void Queue::resume()
{
    if (m_worker->doPause.load())
    {
        m_worker->doPause = false;
        backend_message_queue_start(m_worker->handle);
    }
}
