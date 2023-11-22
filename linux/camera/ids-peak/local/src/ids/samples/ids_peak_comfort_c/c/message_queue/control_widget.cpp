/*!
* \file    control_widget.cpp
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

#include "control_widget.h"
#include "backend.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

ControlWidget::ControlWidget(QWidget* parent)
    : QWidget(parent)
{
    auto layoutControls = new QVBoxLayout(this);
    setLayout(layoutControls);

    auto groupDevice = new QGroupBox(tr("RemoteDevice"), this);
    auto layoutDevice = new QVBoxLayout(groupDevice);
    m_devGroup = new QButtonGroup(groupDevice);
    m_devGroup->setExclusive(false);

    // Add device types
    m_toggles.push_back({ new QCheckBox(tr("RemoteDevice Critical Error"), groupDevice),
        PEAK_MESSAGE_TYPE_REMOTE_DEVICE_CRITICAL_ERROR });
    m_toggles.push_back({ new QCheckBox(tr("RemoteDevice Error"), groupDevice),
        PEAK_MESSAGE_TYPE_REMOTE_DEVICE_ERROR });
    m_toggles.push_back({ new QCheckBox(tr("RemoteDevice Event Dropped"), groupDevice),
        PEAK_MESSAGE_TYPE_REMOTE_DEVICE_EVENT_DROPPED });
    m_toggles.push_back({ new QCheckBox(tr("RemoteDevice Exposure Start"), groupDevice),
        PEAK_MESSAGE_TYPE_REMOTE_DEVICE_EXPOSURE_START });
    m_toggles.push_back({ new QCheckBox(tr("RemoteDevice Exposure End"), groupDevice),
        PEAK_MESSAGE_TYPE_REMOTE_DEVICE_EXPOSURE_END });
    m_toggles.push_back({ new QCheckBox(tr("RemoteDevice Frame Start"), groupDevice),
        PEAK_MESSAGE_TYPE_REMOTE_DEVICE_FRAME_DROPPED });
    m_toggles.push_back({ new QCheckBox(tr("RemoteDevice Temperature"), groupDevice),
        PEAK_MESSAGE_TYPE_REMOTE_DEVICE_TEMPERATURE });
    m_toggles.push_back(
        { new QCheckBox(tr("RemoteDevice Test"), groupDevice), PEAK_MESSAGE_TYPE_REMOTE_DEVICE_TEST });

    m_cameraSelector = new QComboBox(groupDevice);

    m_triggerTest = new QPushButton(tr("Generate Test Event"), groupDevice);
    m_closeCamera = new QPushButton(tr("Close Camera"), groupDevice);
    m_acqCamera = new QPushButton(tr("Acquire Frame"), groupDevice);

    // Add the controls to the layouts
    layoutDevice->addWidget(m_cameraSelector);

    for (auto& t : m_toggles)
    {
        m_devGroup->addButton(t.first);
        layoutDevice->addWidget(t.first);
    }

    layoutControls->addWidget(groupDevice);
    layoutControls->addWidget(m_triggerTest);
    layoutControls->addWidget(m_acqCamera);
    layoutControls->addStretch(1);
    layoutControls->addWidget(m_closeCamera);

    // connect the signals
    connect(
        m_devGroup, qOverload<QAbstractButton*, bool>(&QButtonGroup::buttonToggled), this, &ControlWidget::onDevGroupChanged);
    connect(m_cameraSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &ControlWidget::onCameraChanged);
    connect(m_triggerTest, &QPushButton::clicked, this, &ControlWidget::onTriggerTest);
    connect(m_acqCamera, &QPushButton::clicked, this, &ControlWidget::onAcquireFrame);
    connect(m_closeCamera, &QPushButton::clicked, this, &ControlWidget::onCloseCamera);

    // update values
    updateControl();
}

void ControlWidget::addCamera(const QString& name, peak_camera_id id)
{
    auto hCam = backend_open_camera(id);

    m_cameras.insert(id, Camera{ name, id, hCam });

    m_cameraSelector->addItem(name, static_cast<qulonglong>(id));
}

void ControlWidget::onDevGroupChanged(QAbstractButton* button, bool)
{
    auto id = m_cameraSelector->currentData().toULongLong();

    auto cam = m_cameras.find(static_cast<peak_camera_id>(id));
    if (cam != m_cameras.end())
    {
        auto it = std::find_if(
            m_toggles.begin(), m_toggles.end(), [button](const auto& el) { return el.first == button; });

        if (it != m_toggles.end())
        {
            if (button->isChecked())
            {
                enableMessage(cam->hCam, it->second);
                cam->enabledMessages.push_back(it->second);
            }
            else
            {
                disableMessage(cam->hCam, it->second);
                auto idx = cam->enabledMessages.indexOf(it->second);

                if (idx >= 0)
                {
                    cam->enabledMessages.remove(idx);
                }
            }

            if (it->second == PEAK_MESSAGE_TYPE_REMOTE_DEVICE_TEST)
            {
                m_triggerTest->setEnabled(button->isChecked());
            }
        }
    }
}

void ControlWidget::onCameraChanged(int)
{
    // when camera is changed, update all controls
    updateSupportedMessages();
    updateValues();
    updateControl();
}

void ControlWidget::updateSupportedMessages()
{
    auto id = m_cameraSelector->currentData().toULongLong();

    auto cam = m_cameras.find(static_cast<peak_camera_id>(id));
    if (cam != m_cameras.end())
    {
        for (auto& el : m_toggles)
        {
            auto sup = isMessageSupported(cam->hCam, el.second);
            el.first->setEnabled(sup);
        }
    }
}

void ControlWidget::updateValues()
{
    auto id = m_cameraSelector->currentData().toULongLong();

    auto cam = m_cameras.find(static_cast<peak_camera_id>(id));
    if (cam != m_cameras.end())
    {
        m_devGroup->blockSignals(true);
        for (auto& el : m_toggles)
        {
            auto found = cam->enabledMessages.contains(el.second);
            el.first->setChecked(found);
            if (el.second == PEAK_MESSAGE_TYPE_REMOTE_DEVICE_TEST)
            {
                m_triggerTest->setEnabled(found);
            }
        }
        m_devGroup->blockSignals(false);
    }
    else
    {
        m_devGroup->blockSignals(true);
        for (auto& el : m_toggles)
        {
            el.first->setChecked(false);
        }
        m_devGroup->blockSignals(false);

        m_triggerTest->setEnabled(false);
    }
}

void ControlWidget::updateControl()
{
    auto hasCamera = m_cameraSelector->count() > 0;

    if (!hasCamera)
    {
        for (auto& cb : m_toggles)
        {
            cb.first->setEnabled(hasCamera);

            if (cb.second == PEAK_MESSAGE_TYPE_REMOTE_DEVICE_TEST)
            {
                m_triggerTest->setEnabled(cb.first->isChecked());
            }
        }
    }

    m_acqCamera->setEnabled(hasCamera);
    m_closeCamera->setEnabled(hasCamera);
}

void ControlWidget::onTriggerTest()
{
    auto id = m_cameraSelector->currentData().toULongLong();

    auto cam = m_cameras.find(static_cast<peak_camera_id>(id));
    if (cam != m_cameras.end())
    {
        backend_event_trigger_test(cam->hCam);
    }
}

void ControlWidget::onCloseCamera()
{
    auto id = m_cameraSelector->currentData().toULongLong();

    auto cam = m_cameras.find(static_cast<peak_camera_id>(id));
    if (cam != m_cameras.end())
    {
        backend_close_camera(cam->hCam);
        m_cameras.erase(cam);
    }

    auto idx = m_cameraSelector->currentIndex();
    m_cameraSelector->removeItem(idx);

    updateControl();
}

void ControlWidget::onAcquireFrame()
{
    auto id = m_cameraSelector->currentData().toULongLong();

    auto cam = m_cameras.find(static_cast<peak_camera_id>(id));
    if (cam != m_cameras.end())
    {
        backend_acquisition_single_frame(cam->hCam);
    }
}
