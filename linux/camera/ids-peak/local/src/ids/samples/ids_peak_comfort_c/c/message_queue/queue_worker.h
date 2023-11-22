/*!
 * \file    queue_worker.h
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
#ifndef QUEUE_H
#define QUEUE_H

#include "queue_model.h"
#include <ids_peak_comfort_c/ids_peak_comfort_c.h>
#include <unordered_map>
#include <QObject>
#include <QThread>
#include <algorithm>
#include <atomic>
#include <iostream>
#include <string>
#include <vector>

class QueueWorker : public QThread
{
    Q_OBJECT
public:
    std::atomic_bool doRun{ true };
    std::atomic_bool doPause{ false };

    peak_message_queue_handle handle{};

    void run() override;

signals:
    void newMessage(const MessageData& data);
};

class Queue : public QObject
{
    Q_OBJECT
public:
    explicit Queue(QObject* parent = nullptr);
    ~Queue() override;

    static std::string messageTypeToStr(peak_message_type type);

    void stop();

public slots:
    void pause();
    void resume();
    void enableMessage(void* hCam, int type);
    void disableMessage(void* hCam, int type);

    bool isMessageSupported(void* hCam, int type);

signals:
    void newMessage(const MessageData& data);

private:
    QueueWorker* m_worker{};
};

#endif // QUEUE_H
