/*!
* \file    cameralist_updater.cpp
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

#include "cameralist_updater.h"
#include "backend.h"
#include <QTimer>

void CameraListUpdater::update()
{
    backend_cameralist_update();
}

void CameraListUpdater::start()
{
    m_thread = new QThread(this);
    m_timer = new QTimer();
    m_timer->setInterval(1000);
    m_timer->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_timer, qOverload<>(&QTimer::start));
    connect(m_timer, &QTimer::timeout, this, &CameraListUpdater::update, Qt::DirectConnection);

    QMetaObject::invokeMethod(m_thread, "start", Qt::QueuedConnection);
}

void CameraListUpdater::stop()
{
    if (m_timer)
    {
        QMetaObject::invokeMethod(m_timer, "stop", Qt::QueuedConnection);
    }
    m_thread->quit();
    m_thread->wait();
}

CameraListUpdater::CameraListUpdater(QObject* parent)
    : QObject(parent)
{}

CameraListUpdater::~CameraListUpdater()
{
    stop();
}
