/*!
* \file    cameralist_updater.h
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

#ifndef UPDATER_H
#define UPDATER_H

#include <QThread>
#include <QTimer>

class CameraListUpdater : public QObject
{
    Q_OBJECT
public:
    explicit CameraListUpdater(QObject* parent = nullptr);
    ~CameraListUpdater() override;

    void start();
    void stop();

private slots:
    static void update();

private:
    QThread* m_thread{};
    QTimer* m_timer{};
};


#endif // UPDATER_H
