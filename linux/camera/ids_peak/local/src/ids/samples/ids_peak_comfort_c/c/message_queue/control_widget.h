/*!
* \file    control_widget.h
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

#ifndef CONTROL_H
#define CONTROL_H

#include <ids_peak_comfort_c/ids_peak_comfort_c.h>
#include <QCheckBox>
#include <QComboBox>
#include <QMap>
#include <QPushButton>
#include <QWidget>

struct Camera
{
    QString displayName{};
    peak_camera_id id{};
    peak_camera_handle hCam{};
    QVector<peak_message_type> enabledMessages{};
};

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget* parent = nullptr);

public slots:
    void addCamera(const QString& name, peak_camera_id id);
    void updateControl();

signals:
    void enableMessage(void* hCam, int type);
    void disableMessage(void* hCam, int type);
    bool isMessageSupported(void* hCam, int type);

private slots:
    void onDevGroupChanged(QAbstractButton*, bool);
    void onCameraChanged(int);
    void onTriggerTest();
    void onCloseCamera();
    void onAcquireFrame();

private:
    void updateSupportedMessages();
    void updateValues();

    QWidget* m_controlsWidget{};
    QButtonGroup* m_devGroup{};

    QMap<peak_camera_id, Camera> m_cameras{};

    QVector<QPair<QCheckBox*, peak_message_type>> m_toggles{};
    QComboBox* m_cameraSelector{};
    QPushButton* m_triggerTest{};
    QPushButton* m_closeCamera{};
    QPushButton* m_acqCamera{};
};


#endif // CONTROL_H
