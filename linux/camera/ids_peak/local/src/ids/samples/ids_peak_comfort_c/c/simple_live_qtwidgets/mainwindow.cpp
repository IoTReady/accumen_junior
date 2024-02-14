/*!
 * \file    mainwindow.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.0.0
 *
 * \version 1.2.0
 *
 * Copyright (C) 2022 - 2023, IDS Imaging Development Systems GmbH.
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

#include "mainwindow.h"

#include "backend.h"

#include "acquisitionworker.h"
#include "display.h"

#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QThread>
#include <QWidget>

#include <cstdint>

#define VERSION "1.2.0"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_centralWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_centralWidget);
    setCentralWidget(m_centralWidget);

    // Set minimum window size
    setMinimumSize(600, 400);

    // connect the error callback from backend to MainWindow
    connect(this, &MainWindow::libraryError, this, &MainWindow::HandleLibraryError);
    backend_errorCallback_connect(this, &MainWindow::EmitErrorSignal);

    // Start function of backend: open library, open camera
    int status = backend_start();
    if (status != 0)
    {
        m_hasError = true;
        m_blockErrorMessages = true;
        backend_exit();
        return;
    }

    backend_acquisition_prepare();

    // Create all user interface controls
    ComposeMainWindow();
    CreateControls();

    // Create and start worker thread that will wait for images from the camera
    CreateAcquisitionWorkerThread();
    StartAcquisition();

    m_blockErrorMessages = false;
    m_hasError = false;
}


MainWindow::~MainWindow()
{
    if (m_acquisitionWorker)
    {
        m_acquisitionWorker->Stop();
        m_acquisitionThread.quit();
        m_acquisitionThread.wait();
    }

    backend_exit();
}


bool MainWindow::hasError() const
{
    return m_hasError;
}


void MainWindow::ComposeMainWindow()
{
    m_labelCameraInfo = new QLabel(this);
    m_layout->addWidget(m_labelCameraInfo);
    char modelName[64];
    char serialNumber[64];
    backend_camera_modelName(modelName);
    backend_camera_serialNumber(serialNumber);
    m_labelCameraInfo->setText(
        "Camera " + QString::fromStdString(modelName) + " (" + QString::fromStdString(serialNumber) + ")");

    // Create a display for the camera image
    m_display = new Display(m_centralWidget);
    m_layout->addWidget(m_display);

    auto hLayoutInfo = new QHBoxLayout();
    m_layout->addLayout(hLayoutInfo);

    m_labelInfo = new QLabel(this);
    hLayoutInfo->addWidget(m_labelInfo);
    hLayoutInfo->addStretch();

    auto m_labelVersion = new QLabel(("simple_live_qtwidgets_c v" VERSION), this);
    hLayoutInfo->addWidget(m_labelVersion);

    auto m_labelAboutQt = new QLabel(R"(<a href="#aboutQt">About Qt</a>)", this);
    connect(m_labelAboutQt, &QLabel::linkActivated, this, &MainWindow::OnAboutQtLinkActivated);
    hLayoutInfo->addWidget(m_labelAboutQt);

    show();
}


void MainWindow::CreateControls()
{
    auto controls = new QWidget(this);
    controls->setWindowFlags(Qt::Window);

    auto layoutControls = new QGridLayout();
    controls->setLayout(layoutControls);
    layoutControls->setColumnStretch(1, 1);


    m_buttonStartStop = new QPushButton("Stop acquisition", controls);
    connect(m_buttonStartStop, &QPushButton::clicked, this, &MainWindow::OnButtonStopStart);

    m_labelExposureTime = new QLabel("Exposure time (us)", controls);
    m_sliderExposureTime = new QSlider(Qt::Orientation::Horizontal, controls);
    m_editExposureTime = new QLineEdit(controls);
    m_editExposureTime->setFixedWidth(80);
    connect(m_sliderExposureTime, &QSlider::valueChanged, this, &MainWindow::OnSliderExposureTime);
    connect(m_editExposureTime, &QLineEdit::editingFinished, this, &MainWindow::OnEditExposureTime);

    m_labelFrameRate = new QLabel("Framerate (fps)", controls);
    m_sliderFrameRate = new QSlider(Qt::Orientation::Horizontal, controls);
    m_editFrameRate = new QLineEdit(controls);
    m_editFrameRate->setFixedWidth(80);
    connect(m_sliderFrameRate, &QSlider::valueChanged, this, &MainWindow::OnSliderFrameRate);
    connect(m_editFrameRate, &QLineEdit::editingFinished, this, &MainWindow::OnEditFrameRate);

    m_checkBoxMirrorLeftRight = new QCheckBox("Mirror left/right", controls);
    connect(m_checkBoxMirrorLeftRight, &QCheckBox::stateChanged, this, &MainWindow::OnCheckBoxMirrorLeftRight);

    m_checkBoxMirrorUpDown = new QCheckBox("Mirror up/down", controls);
    connect(m_checkBoxMirrorUpDown, &QCheckBox::stateChanged, this, &MainWindow::OnCheckBoxMirrorUpDown);


    layoutControls->addWidget(m_labelExposureTime, 0, 0);
    layoutControls->addWidget(m_sliderExposureTime, 0, 1);
    layoutControls->addWidget(m_editExposureTime, 0, 2);

    layoutControls->addWidget(m_labelFrameRate, 1, 0);
    layoutControls->addWidget(m_sliderFrameRate, 1, 1);
    layoutControls->addWidget(m_editFrameRate, 1, 2);

    layoutControls->addWidget(m_checkBoxMirrorLeftRight, 3, 0, 1, 3);
    layoutControls->addWidget(m_checkBoxMirrorUpDown, 4, 0);

    layoutControls->addWidget(m_buttonStartStop, 6, 0, 1, 3);

    controls->show();
}


void MainWindow::UpdateExposureTimeControl()
{
    if (backend_exposureTime_isReadable())
    {
        // Block signals from slider and edit to avoid signal loops
        m_sliderExposureTime->blockSignals(true);
        m_editExposureTime->blockSignals(true);

        struct range_double range = backend_exposureTime_range();
        if (range.min == 0 && range.max == 0 && range.inc == 0)
        {
            HandleErrorAndQuit("Unable to read exposure time range.");
        }
        double exposureTime = backend_exposureTime_get();
        if (exposureTime == 0)
        {
            HandleErrorAndQuit("Unable to read exposure time.");
        }

        m_exposureTimeMin = range.min;

        m_exposureTimeInc = range.inc;

        int steps = static_cast<int>((range.max - m_exposureTimeMin) / m_exposureTimeInc);
        m_sliderExposureTime->setRange(0, steps);

        int value = static_cast<int>((exposureTime - m_exposureTimeMin) / m_exposureTimeInc);

        m_sliderExposureTime->setValue(value);

        m_editExposureTime->setText(QString::number(exposureTime, 'f', 2));

        // Re-establish signals from slider and edit
        m_sliderExposureTime->blockSignals(false);
        m_editExposureTime->blockSignals(false);
    }

    if (backend_exposureTime_isWriteable())
    {
        m_labelExposureTime->setEnabled(true);
        m_sliderExposureTime->setEnabled(true);
        m_editExposureTime->setEnabled(true);
    }
    else
    {
        m_labelExposureTime->setEnabled(false);
        m_sliderExposureTime->setEnabled(false);
        m_editExposureTime->setEnabled(false);
    }
}


void MainWindow::UpdateFrameRateControl()
{
    if (backend_frameRate_isReadable())
    {
        // Block signals from slider and edit to avoid signal loops
        m_sliderFrameRate->blockSignals(true);
        m_editFrameRate->blockSignals(true);

        struct range_double range = backend_frameRate_range();
        if (range.min == .0 && range.max == .0 && range.inc == .0)
        {
            HandleErrorAndQuit("Unable to read frame rate range.");
        }
        double frameRate = backend_frameRate_get();
        if (frameRate == 0)
        {
            HandleErrorAndQuit("Unable to read frame rate.");
        }

        m_frameRateMin = range.min;

        m_frameRateInc = range.inc;

        int steps = static_cast<int>((range.max - m_frameRateMin) / m_frameRateInc);
        m_sliderFrameRate->setRange(0, steps);

        int value = static_cast<int>((frameRate - m_frameRateMin) / m_frameRateInc);

        m_sliderFrameRate->setValue(value);

        m_editFrameRate->setText(QString::number(frameRate, 'f', 2));


        // Re-establish signals from slider and edit
        m_sliderFrameRate->blockSignals(false);
        m_editFrameRate->blockSignals(false);
    }

    if (backend_frameRate_isWriteable())
    {
        m_labelFrameRate->setEnabled(true);
        m_sliderFrameRate->setEnabled(true);
        m_editFrameRate->setEnabled(true);
    }
    else
    {
        m_labelFrameRate->setEnabled(false);
        m_sliderFrameRate->setEnabled(false);
        m_editFrameRate->setEnabled(false);
    }
}


void MainWindow::UpdateMirrorControls()
{
    if (backend_mirrorLeftRight_isReadable())
    {
        m_checkBoxMirrorLeftRight->setChecked(backend_mirrorLeftRight_isEnabled());
    }

    if (backend_mirrorLeftRight_isWriteable())
    {
        m_checkBoxMirrorLeftRight->setEnabled(true);
    }
    else
    {
        m_checkBoxMirrorLeftRight->setEnabled(false);
    }

    if (backend_mirrorUpDown_isReadable())
    {
        m_checkBoxMirrorUpDown->setChecked(backend_mirrorUpDown_isEnabled());
    }

    if (backend_mirrorUpDown_isWriteable())
    {
        m_checkBoxMirrorUpDown->setEnabled(true);
    }
    else
    {
        m_checkBoxMirrorUpDown->setEnabled(false);
    }
}


void MainWindow::CreateAcquisitionWorkerThread()
{
    // Create worker thread that waits for new images from the camera
    m_acquisitionWorker = new AcquisitionWorker();
    m_acquisitionWorker->moveToThread(&m_acquisitionThread);

    // Worker must be started, when the acquisition starts
    connect(&m_acquisitionThread, SIGNAL(started()), m_acquisitionWorker, SLOT(Start()), Qt::UniqueConnection);

    // Connect the signal from the worker thread when a new image was received with the display update slot in the
    // Display class
    connect(m_acquisitionWorker, &AcquisitionWorker::imageReceived, m_display, &Display::OnImageReceived);

    // Connect the signal from the worker thread when the counters have changed with the update slot in the MainWindow
    // class
    connect(m_acquisitionWorker, &AcquisitionWorker::counterUpdated, this, &MainWindow::OnCounterUpdated);

    // Connect the signal from the worker thread when the acqusitions was started
    connect(m_acquisitionWorker, &AcquisitionWorker::started, this, &MainWindow::UpdateControls);

    // Connect the signal from the worker thread when the acqusitions was stopped
    connect(m_acquisitionWorker, &AcquisitionWorker::stopped, this, &MainWindow::UpdateControls);

    // Connect error signal of worker thread with slot in the MainWindow class
    connect(m_acquisitionWorker, &AcquisitionWorker::error, this, &MainWindow::HandleErrorAndQuit);
}


void MainWindow::StartAcquisition()
{
    // Start thread execution.
    backend_acquisition_start();

    m_acquisitionThread.start();
}


void MainWindow::StopAcquisition()
{
    if (m_acquisitionWorker)
    {
        m_acquisitionWorker->Stop();
        m_acquisitionThread.quit();
        m_acquisitionThread.wait();

        backend_acquisition_stop();
    }
}


void MainWindow::OnButtonStopStart()
{
    if ("Stop acquisition" == m_buttonStartStop->text())
    {
        m_buttonStartStop->setText("Start acquisition");
        StopAcquisition();
    }
    else
    {
        m_buttonStartStop->setText("Stop acquisition");
        StartAcquisition();
    }
}


void MainWindow::OnSliderExposureTime()
{
    int value = m_sliderExposureTime->value();

    double exposureTime = m_exposureTimeMin + static_cast<double>(value) * m_exposureTimeInc;

    if (backend_exposureTime_isWriteable())
    {
        backend_exposureTime_set(exposureTime);

        exposureTime = backend_exposureTime_get();

        m_editExposureTime->blockSignals(true);
        m_editExposureTime->setText(QString::number(exposureTime, 'f', 2));
        m_editExposureTime->blockSignals(false);
    }
    UpdateFrameRateControl();
}


void MainWindow::OnEditExposureTime()
{
    double exposureTime = m_editExposureTime->text().toDouble();

    if (backend_exposureTime_isWriteable())
    {
        backend_exposureTime_set(exposureTime);

        exposureTime = backend_exposureTime_get();

        int value = static_cast<int>((exposureTime - m_exposureTimeMin) / m_exposureTimeInc);

        m_sliderExposureTime->blockSignals(true);
        m_sliderExposureTime->setValue(value);
        m_sliderExposureTime->blockSignals(false);
    }
    UpdateFrameRateControl();
}


void MainWindow::OnSliderFrameRate()
{
    int value = m_sliderFrameRate->value();

    double frameRate = m_frameRateMin + static_cast<double>(value) * m_frameRateInc;

    if (backend_frameRate_isWriteable())
    {
        backend_frameRate_set(frameRate);

        frameRate = backend_frameRate_get();

        m_editFrameRate->blockSignals(true);
        m_editFrameRate->setText(QString::number(frameRate, 'f', 2));
        m_editFrameRate->blockSignals(false);
    }
}


void MainWindow::OnEditFrameRate()
{
    double frameRate = m_editFrameRate->text().toDouble();

    if (backend_frameRate_isWriteable())
    {
        backend_frameRate_set(frameRate);

        frameRate = backend_frameRate_get();

        int value = static_cast<int>((frameRate - m_frameRateMin) / m_frameRateInc);

        m_sliderFrameRate->blockSignals(true);
        m_sliderFrameRate->setValue(value);
        m_sliderFrameRate->blockSignals(false);
    }
}


void MainWindow::UpdateControls()
{
    UpdateExposureTimeControl();
    UpdateFrameRateControl();
    UpdateMirrorControls();
}


void MainWindow::OnCheckBoxMirrorLeftRight()
{
    if (backend_mirrorLeftRight_isWriteable())
    {
        backend_mirrorLeftRight_enable(m_checkBoxMirrorLeftRight->isChecked());
    }
}


void MainWindow::OnCheckBoxMirrorUpDown()
{
    if (backend_mirrorUpDown_isWriteable())
    {
        backend_mirrorUpDown_enable(m_checkBoxMirrorUpDown->isChecked());
    }
}


void MainWindow::OnCounterUpdated(unsigned int frameCounter, unsigned int errorCounter)
{
    QString strText;
    strText.sprintf("Acquired: %d, errors: %d", frameCounter, errorCounter);
    m_labelInfo->setText(strText);
}


void MainWindow::OnAboutQtLinkActivated(const QString& link)
{
    if (link == "#aboutQt")
    {
        QMessageBox::aboutQt(this, "About Qt");
    }
}


void MainWindow::EmitErrorSignal(const char* message, int status, void* errorCallbackContext)
{
    emit static_cast<MainWindow*>(errorCallbackContext)->libraryError(QString::fromStdString(message), status);
};


void MainWindow::HandleLibraryError(QString message, int) const
{
    // block all error messages due to a critical message
    if (m_blockErrorMessages)
    {
        return;
    }

    message.prepend("Error: ");

    QMessageBox::warning(nullptr, "Error", message, QMessageBox::Ok);
}


void MainWindow::HandleErrorAndQuit(QString message) const
{
    // block all error messages due to a critical message
    if (m_blockErrorMessages)
    {
        return;
    }

    message += "\n(Error!)";
    message += "\nExiting application";

    QMessageBox::critical(nullptr, "Error", message, QMessageBox::Ok);
    qApp->quit();
}
