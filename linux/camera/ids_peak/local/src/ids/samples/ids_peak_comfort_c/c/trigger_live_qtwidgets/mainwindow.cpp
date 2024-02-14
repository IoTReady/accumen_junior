/*!
 * \file    mainwindow.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-12-01
 * \since   2.0.0
 *
 * \version 1.2.1
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
#include <QThread>
#include <QWidget>

#include <cstdint>

#define VERSION "1.2.1"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_frameRateLast{ 0.0 }
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
    if (backend_acquisition_isStarted())
    {
        backend_acquisition_stop();
    }

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

    auto m_labelVersion = new QLabel(("trigger_live_qtwidgets_c v" VERSION), this);
    hLayoutInfo->addWidget(m_labelVersion);

    auto m_labelAboutQt = new QLabel(R"(<a href="#aboutQt">About Qt</a>)", this);
    connect(m_labelAboutQt, &QLabel::linkActivated, this, &MainWindow::OnAboutQtLinkActivated);
    hLayoutInfo->addWidget(m_labelAboutQt);

    show();
}


void MainWindow::CreateControls()
{
    m_controlsWidget = new QWidget(this);
    m_controlsWidget->setWindowFlags(Qt::Window);

    auto layoutControls = new QVBoxLayout();
    m_controlsWidget->setLayout(layoutControls);
    layoutControls->setSpacing(24);

    m_frameTriggerControls = new QFrame(this);
    m_frameTriggerControls->setFrameStyle(QFrame::NoFrame);

    m_layoutTriggerControls = new QGridLayout();
    m_layoutTriggerControls->setColumnStretch(1, 1);
    m_layoutTriggerControls->setMargin(0);
    m_frameTriggerControls->setLayout(m_layoutTriggerControls);

    m_frameFreerunControls = new QFrame(this);
    m_frameFreerunControls->setFrameStyle(QFrame::NoFrame);

    m_layoutFreerunControls = new QGridLayout();
    m_layoutFreerunControls->setColumnStretch(1, 1);
    m_layoutFreerunControls->setMargin(0);
    m_frameFreerunControls->setLayout(m_layoutFreerunControls);

    m_radioFreerun = new QRadioButton("Freerun", this);
    connect(m_radioFreerun, &QRadioButton::clicked, this, &MainWindow::OnRadioFreerun);

    m_radioSoftwareTrigger = new QRadioButton("Software trigger", this);
    connect(m_radioSoftwareTrigger, &QRadioButton::clicked, this, &MainWindow::OnRadioSoftwareTrigger);

    m_radioHardwareTrigger = new QRadioButton("Hardware trigger (trigger input)", this);
    connect(m_radioHardwareTrigger, &QRadioButton::clicked, this, &MainWindow::OnRadioHardwareTrigger);

    m_radioHardwareTriggerGPIO1 = new QRadioButton("Hardware trigger (GPIO 1)", this);
    connect(m_radioHardwareTriggerGPIO1, &QRadioButton::clicked, this, &MainWindow::OnRadioHardwareTriggerGPIO1);

    m_radioHardwareTriggerGPIO2 = new QRadioButton("Hardware trigger (GPIO 2)", this);
    connect(m_radioHardwareTriggerGPIO2, &QRadioButton::clicked, this, &MainWindow::OnRadioHardwareTriggerGPIO2);

    // The button group handles the exclusive activation of radio buttons
    m_radioButtonGroup = new QButtonGroup(this);
    m_radioButtonGroup->setExclusive(true);
    m_radioButtonGroup->addButton(m_radioFreerun);
    m_radioButtonGroup->addButton(m_radioSoftwareTrigger);
    m_radioButtonGroup->addButton(m_radioHardwareTrigger);
    m_radioButtonGroup->addButton(m_radioHardwareTriggerGPIO1);
    m_radioButtonGroup->addButton(m_radioHardwareTriggerGPIO2);

    m_buttonStartStop = new QPushButton("Stop acquisition", m_controlsWidget);
    connect(m_buttonStartStop, &QPushButton::clicked, this, &MainWindow::OnButtonStopStart);

    m_buttonTrigger = new QPushButton("Execute trigger", m_controlsWidget);
    connect(m_buttonTrigger, &QPushButton::clicked, this, &MainWindow::OnButtonTrigger);

    m_labelTriggerDelay = new QLabel("Trigger Delay (us)", m_controlsWidget);
    m_sliderTriggerDelay = new QSlider(Qt::Orientation::Horizontal, m_controlsWidget);
    m_editTriggerDelay = new QLineEdit(m_controlsWidget);
    m_editTriggerDelay->setFixedWidth(80);
    connect(m_sliderTriggerDelay, &QSlider::valueChanged, this, &MainWindow::OnSliderTriggerDelay);
    connect(m_editTriggerDelay, &QLineEdit::editingFinished, this, &MainWindow::OnEditTriggerDelay);

    m_labelTriggerDivider = new QLabel("Trigger Divider", m_controlsWidget);
    m_sliderTriggerDivider = new QSlider(Qt::Orientation::Horizontal, m_controlsWidget);
    m_editTriggerDivider = new QLineEdit(m_controlsWidget);
    m_editTriggerDivider->setFixedWidth(80);
    connect(m_sliderTriggerDivider, &QSlider::valueChanged, this, &MainWindow::OnSliderTriggerDivider);
    connect(m_editTriggerDivider, &QLineEdit::editingFinished, this, &MainWindow::OnEditTriggerDivider);

    m_labelTriggerEdge = new QLabel("Trigger Edge", m_controlsWidget);
    m_comboTriggerEdge = new QComboBox(m_controlsWidget);
    connect(m_comboTriggerEdge, &QComboBox::currentTextChanged, this, &MainWindow::OnComboTriggerEdge);

    m_labelExposureTime = new QLabel("Exposure time (us)", m_controlsWidget);
    m_sliderExposureTime = new QSlider(Qt::Orientation::Horizontal, m_controlsWidget);
    m_editExposureTime = new QLineEdit(m_controlsWidget);
    m_editExposureTime->setFixedWidth(80);
    connect(m_sliderExposureTime, &QSlider::valueChanged, this, &MainWindow::OnSliderExposureTime);
    connect(m_editExposureTime, &QLineEdit::editingFinished, this, &MainWindow::OnEditExposureTime);

    m_labelFrameRate = new QLabel("Framerate (fps)", m_controlsWidget);
    m_sliderFrameRate = new QSlider(Qt::Orientation::Horizontal, m_controlsWidget);
    m_editFrameRate = new QLineEdit(m_controlsWidget);
    m_editFrameRate->setFixedWidth(80);
    connect(m_sliderFrameRate, &QSlider::valueChanged, this, &MainWindow::OnSliderFrameRate);
    connect(m_editFrameRate, &QLineEdit::editingFinished, this, &MainWindow::OnEditFrameRate);

    // The group boxes display the controls in separate boxes
    m_generalGroupBox = new QGroupBox("General");
    m_generalLayout = new QGridLayout(m_generalGroupBox);

    m_acquisitionGroupBox = new QGroupBox("Acquisition / trigger type");
    m_acquisitionLayout = new QVBoxLayout(m_acquisitionGroupBox);

    m_optionsGroupBox = new QGroupBox("Acquisition options");
    m_optionsLayout = new QVBoxLayout(m_optionsGroupBox);

    // Add the controls to the layouts
    m_generalLayout->addWidget(m_labelExposureTime, 0, 0);
    m_generalLayout->addWidget(m_sliderExposureTime, 0, 1);
    m_generalLayout->addWidget(m_editExposureTime, 0, 2);
    m_generalLayout->addWidget(m_buttonStartStop, 1, 0, 1, 3);

    layoutControls->addWidget(m_generalGroupBox);

    m_acquisitionLayout->addWidget(m_radioFreerun);
    m_acquisitionLayout->addWidget(m_radioSoftwareTrigger);
    m_acquisitionLayout->addWidget(m_radioHardwareTrigger);
    m_acquisitionLayout->addWidget(m_radioHardwareTriggerGPIO1);
    m_acquisitionLayout->addWidget(m_radioHardwareTriggerGPIO2);

    layoutControls->addWidget(m_acquisitionGroupBox);

    m_layoutFreerunControls->addWidget(m_labelFrameRate, 0, 0);
    m_layoutFreerunControls->addWidget(m_sliderFrameRate, 0, 1);
    m_layoutFreerunControls->addWidget(m_editFrameRate, 0, 2);

    m_layoutTriggerControls->addWidget(m_labelTriggerDelay, 0, 0);
    m_layoutTriggerControls->addWidget(m_sliderTriggerDelay, 0, 1);
    m_layoutTriggerControls->addWidget(m_editTriggerDelay, 0, 2);
    m_layoutTriggerControls->addWidget(m_labelTriggerDivider, 1, 0);
    m_layoutTriggerControls->addWidget(m_sliderTriggerDivider, 1, 1);
    m_layoutTriggerControls->addWidget(m_editTriggerDivider, 1, 2);
    m_layoutTriggerControls->addWidget(m_labelTriggerEdge, 2, 0);
    m_layoutTriggerControls->addWidget(m_comboTriggerEdge, 2, 1, 1, 2);
    m_layoutTriggerControls->addWidget(m_buttonTrigger, 3, 0, 1, 3);

    m_optionsLayout->addWidget(m_frameFreerunControls);
    m_optionsLayout->addWidget(m_frameTriggerControls);

    layoutControls->addWidget(m_optionsGroupBox);
    layoutControls->addStretch();

    m_frameFreerunControls->adjustSize();
    m_frameTriggerControls->adjustSize();

    UpdateControls();
    m_controlsWidget->show();
}


void MainWindow::UpdateTriggerChannel()
{
    if (!backend_trigger_isEnabled())
    {
        m_radioFreerun->setChecked(true);
        return;
    }

    auto currentTrigger = backend_triggerChannel_get();
    switch (currentTrigger)
    {
    case 0:
        m_radioSoftwareTrigger->setChecked(true);
        break;
    case 1:
        m_radioHardwareTrigger->setChecked(true);
        break;
    case 2:
        m_radioHardwareTriggerGPIO1->setChecked(true);
        break;
    case 3:
        m_radioHardwareTriggerGPIO2->setChecked(true);
        break;
    default:
        m_radioFreerun->setChecked(true);
        break;
    }
}


void MainWindow::UpdateTriggerDelayControl()
{
    if (backend_triggerDelay_isReadable())
    {
        // Block signals from slider and edit to avoid signal loops
        m_sliderTriggerDelay->blockSignals(true);
        m_editTriggerDelay->blockSignals(true);

        struct range_double range = backend_triggerDelay_range();
        if (range.min == 0 && range.max == 0 && range.inc == 0)
        {
            HandleErrorAndQuit("Unable to read trigger delay range.");
        }
        double triggerDelay = backend_triggerDelay_get();
        if (triggerDelay == -1)
        {
            HandleErrorAndQuit("Unable to read trigger delay.");
        }

        m_triggerDelayMin = range.min;

        m_triggerDelayInc = range.inc;

        int steps = static_cast<int>((range.max - m_triggerDelayMin) / m_triggerDelayInc);
        m_sliderTriggerDelay->setRange(0, steps);

        int value = static_cast<int>((triggerDelay - m_triggerDelayMin) / m_triggerDelayInc);

        m_sliderTriggerDelay->setValue(value);

        m_editTriggerDelay->setText(QString::number(triggerDelay, 'f', 2));

        // Re-establish signals from slider and edit
        m_sliderTriggerDelay->blockSignals(false);
        m_editTriggerDelay->blockSignals(false);
    }

    if (backend_triggerDelay_isWriteable())
    {
        m_labelTriggerDelay->setEnabled(true);
        m_sliderTriggerDelay->setEnabled(true);
        m_editTriggerDelay->setEnabled(true);
    }
    else
    {
        m_labelTriggerDelay->setEnabled(false);
        m_sliderTriggerDelay->setEnabled(false);
        m_editTriggerDelay->setEnabled(false);
    }
}


void MainWindow::UpdateTriggerDividerControl()
{
    if (backend_triggerDivider_isReadable())
    {
        // Block signals from slider and edit to avoid signal loops
        m_sliderTriggerDivider->blockSignals(true);
        m_editTriggerDivider->blockSignals(true);

        struct range_int range = backend_triggerDivider_range();
        if (range.min == 0 && range.max == 0 && range.inc == 0)
        {
            HandleErrorAndQuit("Unable to read trigger divider range.");
        }
        uint32_t triggerDivider = backend_triggerDivider_get();

        m_triggerDividerMin = range.min;

        m_triggerDividerInc = range.inc;

        auto steps = (range.max - m_triggerDividerMin) / m_triggerDividerInc;
        m_sliderTriggerDivider->setRange(0, static_cast<int>(steps));

        auto value = (triggerDivider - m_triggerDividerMin) / m_triggerDividerInc;

        m_sliderTriggerDivider->setValue(static_cast<int>(value));

        m_editTriggerDivider->setText(QString::number(triggerDivider));

        // Re-establish signals from slider and edit
        m_sliderTriggerDivider->blockSignals(false);
        m_editTriggerDivider->blockSignals(false);
    }

    if (backend_triggerDivider_isWriteable())
    {
        m_labelTriggerDivider->setEnabled(true);
        m_sliderTriggerDivider->setEnabled(true);
        m_editTriggerDivider->setEnabled(true);
    }
    else
    {
        m_labelTriggerDivider->setEnabled(false);
        m_sliderTriggerDivider->setEnabled(false);
        m_editTriggerDivider->setEnabled(false);
    }
}


void MainWindow::UpdateTriggerEdgeControl()
{
    if (backend_triggerEdge_isReadable())
    {
        struct edge_available availableEdges = backend_triggerEdge_getAvailable();

        m_comboTriggerEdge->clear();

        if (availableEdges.rising)
        {
            m_comboTriggerEdge->addItem("Rising edge");
        }
        if (availableEdges.falling)
        {
            m_comboTriggerEdge->addItem("Falling edge");
        }
        if (availableEdges.any)
        {
            m_comboTriggerEdge->addItem("Any edge");
        }

        int currentEntry = backend_triggerEdge_get();
        switch (currentEntry) // NOLINT
        {
        case 0:
            m_comboTriggerEdge->setCurrentText("Rising edge");
            break;
        case 1:
            m_comboTriggerEdge->setCurrentText("Falling edge");
            break;
        case 2:
            m_comboTriggerEdge->setCurrentText("Any edge");
            break;
        }
    }

    if (backend_triggerEdge_isWriteable() && (m_comboTriggerEdge->count() > 0))
    {
        m_labelTriggerEdge->setEnabled(true);
        m_comboTriggerEdge->setEnabled(true);
    }
    else
    {
        m_labelTriggerEdge->setEnabled(false);
        m_comboTriggerEdge->setEnabled(false);
    }
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
        m_frameRate = backend_frameRate_get();
        if (m_frameRate == 0.0)
        {
            HandleErrorAndQuit("Unable to read frame rate.");
        }

        m_frameRateMin = range.min;

        m_frameRateInc = range.inc;

        int steps = static_cast<int>((range.max - m_frameRateMin) / m_frameRateInc);
        m_sliderFrameRate->setRange(0, steps);

        int value = static_cast<int>((m_frameRate - m_frameRateMin) / m_frameRateInc);

        m_sliderFrameRate->setValue(value);

        m_editFrameRate->setText(QString::number(m_frameRate, 'f', 2));


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


void MainWindow::UpdateTriggerExecuteControl()
{
    if (backend_acquisition_isStarted())
    {
        m_buttonTrigger->setEnabled(backend_trigger_isExecutable());
    }
    else
    {
        m_buttonTrigger->setEnabled(false);
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
    UpdateControls();
}


void MainWindow::StopAcquisition()
{
    if (m_acquisitionWorker)
    {
        m_acquisitionWorker->Stop();

        backend_acquisition_stop();

        m_acquisitionThread.quit();
        m_acquisitionThread.wait();
    }
    UpdateControls();
}


void MainWindow::OnButtonStopStart()
{
    if (backend_acquisition_isStarted())
    {
        StopAcquisition();
    }
    else
    {
        StartAcquisition();
    }
    UpdateControls();
}


void MainWindow::OnButtonTrigger()
{
    backend_trigger_execute();
}


void MainWindow::OnRadioFreerun(bool state)
{
    if (state)
    {
        backend_prepare_freerun();

        // Restore last framerate
        if (m_frameRateLast != 0.0)
        {
            backend_frameRate_set(m_frameRateLast);
            UpdateFrameRateControl();
        }
    }
    else
    {
        // Remember last framerate
        m_frameRateLast = m_frameRate;
    }
    UpdateControls();
}


void MainWindow::OnRadioSoftwareTrigger(bool state)
{
    if (state)
    {
        backend_triggerChannel_set(0);
    }
    UpdateControls();
}


void MainWindow::OnRadioHardwareTrigger(bool state)
{
    if (state)
    {
        backend_triggerChannel_set(1);
    }
    UpdateControls();
}


void MainWindow::OnRadioHardwareTriggerGPIO1(bool state)
{
    if (state)
    {
        backend_triggerChannel_set(2);
    }
    UpdateControls();
}


void MainWindow::OnRadioHardwareTriggerGPIO2(bool state)
{
    if (state)
    {
        backend_triggerChannel_set(3);
    }
    UpdateControls();
}


void MainWindow::OnSliderTriggerDelay()
{
    int value = m_sliderTriggerDelay->value();

    double triggerDelay = m_triggerDelayMin + static_cast<double>(value) * m_triggerDelayInc;

    if (backend_triggerDelay_isWriteable())
    {
        backend_triggerDelay_set(triggerDelay);

        triggerDelay = backend_triggerDelay_get();

        m_editTriggerDelay->blockSignals(true);
        m_editTriggerDelay->setText(QString::number(triggerDelay, 'f', 2));
        m_editTriggerDelay->blockSignals(false);
    }
}


void MainWindow::OnEditTriggerDelay()
{
    double triggerDelay = m_editTriggerDelay->text().toDouble();

    if (backend_triggerDelay_isWriteable())
    {
        backend_triggerDelay_set(triggerDelay);

        triggerDelay = backend_triggerDelay_get();

        int value = static_cast<int>((triggerDelay - m_triggerDelayMin) / m_triggerDelayInc);

        m_sliderTriggerDelay->blockSignals(true);
        m_sliderTriggerDelay->setValue(value);
        m_sliderTriggerDelay->blockSignals(false);
    }
}


void MainWindow::OnSliderTriggerDivider()
{
    int value = m_sliderTriggerDivider->value();

    uint32_t triggerDivider = m_triggerDividerMin + value * m_triggerDividerInc;

    if (backend_triggerDivider_isWriteable())
    {
        backend_triggerDivider_set(triggerDivider);

        triggerDivider = backend_triggerDivider_get();

        m_editTriggerDivider->blockSignals(true);
        m_editTriggerDivider->setText(QString::number(triggerDivider));
        m_editTriggerDivider->blockSignals(false);
    }
}


void MainWindow::OnEditTriggerDivider()
{
    uint32_t triggerDivider = m_editTriggerDivider->text().toUInt();

    if (backend_triggerDivider_isWriteable())
    {
        backend_triggerDivider_set(triggerDivider);

        triggerDivider = backend_triggerDivider_get();

        int value = static_cast<int>((triggerDivider - m_triggerDividerMin) / m_triggerDividerInc);

        m_sliderTriggerDivider->blockSignals(true);
        m_sliderTriggerDivider->setValue(value);
        m_sliderTriggerDivider->blockSignals(false);
    }
}


void MainWindow::OnComboTriggerEdge(const QString& text)
{
    if (backend_triggerEdge_isWriteable())
    {
        if (text == "Rising edge")
        {
            backend_triggerEdge_set(0);
        }
        else if (text == "Falling edge")
        {
            backend_triggerEdge_set(1);
        }
        else if (text == "Any edge")
        {
            backend_triggerEdge_set(2);
        }
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

    m_frameRate = m_frameRateMin + static_cast<double>(value) * m_frameRateInc;

    if (backend_frameRate_isWriteable())
    {
        backend_frameRate_set(m_frameRate);

        m_frameRate = backend_frameRate_get();

        m_editFrameRate->blockSignals(true);
        m_editFrameRate->setText(QString::number(m_frameRate, 'f', 2));
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
    UpdateTriggerChannel();

    if (m_radioFreerun->isChecked())
    {
        // Show all freerun controls
        m_frameFreerunControls->show();

        // Hide all trigger controls
        m_frameTriggerControls->hide();
    }
    else
    {
        // Hide all freerun controls
        m_frameFreerunControls->hide();

        // Show all trigger controls
        m_frameTriggerControls->show();
    }

    if (backend_acquisition_isStarted())
    {
        m_buttonStartStop->setText("Stop acquisition");
        for (const auto& button : m_radioButtonGroup->buttons())
        {
            button->setEnabled(false);
        }
    }
    else
    {
        m_buttonStartStop->setText("Start acquisition");
        for (const auto& button : m_radioButtonGroup->buttons())
        {
            button->setEnabled(true);
        }

        if (backend_triggerChannel_isWriteable())
        {
            struct channel_available availableChannels = backend_triggerChannel_getAvailable();
            m_radioSoftwareTrigger->setEnabled(availableChannels.software);
            m_radioHardwareTrigger->setEnabled(availableChannels.hardware);
            m_radioHardwareTriggerGPIO1->setEnabled(availableChannels.gpio1);
            m_radioHardwareTriggerGPIO2->setEnabled(availableChannels.gpio2);
        }
        else
        {
            m_radioSoftwareTrigger->setEnabled(false);
            m_radioHardwareTrigger->setEnabled(false);
            m_radioHardwareTriggerGPIO1->setEnabled(false);
            m_radioHardwareTriggerGPIO2->setEnabled(false);
        }
    }

    UpdateExposureTimeControl();

    UpdateFrameRateControl();

    UpdateTriggerExecuteControl();
    UpdateTriggerDelayControl();
    UpdateTriggerDividerControl();
    UpdateTriggerEdgeControl();
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
        return;

    message.prepend("Error: ");

    QMessageBox::warning(nullptr, "Error", message, QMessageBox::Ok);
}


void MainWindow::HandleErrorAndQuit(QString message) const
{
    // block all error messages due to a critical message
    if (m_blockErrorMessages)
        return;

    message += "\n(Error!)";
    message += "\nExiting application";

    QMessageBox::critical(nullptr, "Error", message, QMessageBox::Ok);
    qApp->quit();
}