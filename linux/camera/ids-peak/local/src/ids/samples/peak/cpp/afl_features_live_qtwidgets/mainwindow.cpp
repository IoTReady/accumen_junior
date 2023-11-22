/*!
 * \file    mainwindow.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-03-07
 * \since   1.0.0
 *
 * \version 1.2.0
 *
 * Copyright (C) 2019 - 2023, IDS Imaging Development Systems GmbH.
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

#include "acquisitionworker.h"
#include "display.h"

#include <QApplication>
#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QWidget>

#include <cmath>
#include <cstdint>

#define VERSION "1.0"

const double US_TO_MS_DENOMINATOR = 1000.0;


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_layout(new QVBoxLayout())
{
    auto* m_centralWidget = new QWidget(this);
    m_centralWidget->setLayout(m_layout);
    setCentralWidget(m_centralWidget);

    // Connect the signal from the backend when an exception was thrown and a message should be printed
    // with the messagebox show slot in the MainWindow class
    connect(&m_backEnd, &BackEnd::MessageBoxTrigger, &MainWindow::ShowMessageBox);

    if (m_backEnd.Start())
    {
        // Create a display for the camera image
        m_display = new Display(m_centralWidget);
        m_layout->addWidget(m_display);
        connect(m_display, &Display::newROI, this, &MainWindow::OnNewROI);

        // Connect the signal from the backend when a new image was received with the display update slot in
        // the Display class
        connect(&m_backEnd, &BackEnd::ImageReceived, m_display, &Display::onImageReceived);

        // Connect the signal from the backend when the counters have changed with the update slot in the
        // MainWindow class
        connect(&m_backEnd, &BackEnd::CounterChanged, this, &MainWindow::OnCounterChanged);

        CreateStatusBar();
        CreateControls();
        InitializeControls();
        ConnectControls();

        // Set minimum window size
        const int MINIMUM_WINDOW_WIDTH = 700;
        const int MINIMUM_WINDOW_HEIGHT = 500;
        this->setMinimumSize(MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);

        m_hasError = false;
    }
    else
    {
        m_hasError = true;
    }
}


void MainWindow::CreateStatusBar()
{
    auto* statusBar = new QWidget(centralWidget());
    auto* layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    m_labelInfo = new QLabel(statusBar);
    m_labelInfo->setAlignment(Qt::AlignLeft);
    layout->addWidget(m_labelInfo);
    layout->addStretch();

    auto* m_labelVersion = new QLabel(statusBar);
    m_labelVersion->setText(("afl_features_live_qtwidgets v" VERSION));
    m_labelVersion->setAlignment(Qt::AlignRight);
    layout->addWidget(m_labelVersion);

    auto* m_labelAboutQt = new QLabel(statusBar);
    m_labelAboutQt->setObjectName("aboutQt");
    m_labelAboutQt->setText(R"(<a href="#aboutQt">About Qt</a>)");
    m_labelAboutQt->setAlignment(Qt::AlignRight);
    connect(m_labelAboutQt, &QLabel::linkActivated, this, &MainWindow::On_aboutQt_linkActivated);
    layout->addWidget(m_labelAboutQt);
    statusBar->setLayout(layout);

    m_layout->addWidget(statusBar);
}


bool MainWindow::HasError() const
{
    return m_hasError;
}

void MainWindow::ShowMessageBox(const QString& messageTitle, const QString& messageText)
{
    // Close existing messageboxes to avoid multiple messageboxes
    QWidgetList topWidgets = QApplication::topLevelWidgets();
    for (QWidget* w : topWidgets)
    {
        if (auto* mBox = qobject_cast<QMessageBox*>(w))
        {
            mBox->close();
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle(messageTitle);
    msgBox.setText(messageText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::CreateControls()
{
    auto* m_controlsWidget = new QWidget(this);
    m_controlsWidget->setWindowFlags(Qt::Window);

    auto vLayoutControl = new QVBoxLayout(m_controlsWidget);

    // Exposure slider
    auto generalGridLayout = new QGridLayout();
    generalGridLayout->setSpacing(6);

    // NOTE: When you use a layout, you do not need to pass a parent when constructing
    // the child widgets. The layout will automatically reparent the widgets
    // (using QWidget::setParent()) so that they are children of the widget on which
    // the layout is installed.
    auto labelExposureSlider = new QLabel("Exposure [ms]");
    m_exposureSlider = new QSlider(Qt::Orientation::Horizontal);

    // Exposure value is displayed in ms instead of us
    m_exposureValueEdit = new QLineEdit("NA");

    generalGridLayout->addWidget(labelExposureSlider, 0, 0, Qt::AlignLeft);
    generalGridLayout->addWidget(m_exposureSlider, 0, 1, Qt::Alignment());
    generalGridLayout->addWidget(m_exposureValueEdit, 0, 2, Qt::AlignRight);

    // Gain slider
    auto labelGainSlider = new QLabel("Gain");
    m_gainSlider = new QSlider(Qt::Orientation::Horizontal);

    m_gainValueEdit = new QLineEdit("1");

    generalGridLayout->addWidget(labelGainSlider, 1, 0, Qt::AlignLeft);
    generalGridLayout->addWidget(m_gainSlider, 1, 1, Qt::Alignment());
    generalGridLayout->addWidget(m_gainValueEdit, 1, 2, Qt::AlignRight);

    // Frame rate slider
    auto labelFPSSlider = new QLabel("Frame rate [fps]");
    m_fpsSlider = new QSlider(Qt::Orientation::Horizontal);
    m_fpsValueEdit = new QLineEdit("NA", m_controlsWidget);

    generalGridLayout->addWidget(labelFPSSlider, 2, 0, Qt::AlignLeft);
    generalGridLayout->addWidget(m_fpsSlider, 2, 1, Qt::Alignment());
    generalGridLayout->addWidget(m_fpsValueEdit, 2, 2, Qt::AlignRight);

    auto autoFocusLayout = new QGridLayout();
    autoFocusLayout->setSpacing(6);
    // Autofocus
    auto labelFocusMode = new QLabel("Autofocus");
    m_comboFocusMode = new QComboBox();
    m_comboFocusMode->addItems({ "Off", "Once", "Continuous" });

    autoFocusLayout->addWidget(labelFocusMode, 0, 0, Qt::AlignLeft);
    autoFocusLayout->addWidget(m_comboFocusMode, 0, 1, Qt::AlignLeft);

    // Focus Search Algorithm
    auto labelSearchAlgo = new QLabel("Search algorithm");
    m_comboSearchAlgo = new QComboBox();
    // so the size gets updated when we populate the values later
    m_comboSearchAlgo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);

    autoFocusLayout->addWidget(labelSearchAlgo, 1, 0, Qt::AlignLeft);
    autoFocusLayout->addWidget(m_comboSearchAlgo, 1, 1, Qt::AlignLeft);

    // Sharpness Algorithm
    auto labelSharpnessAlgo = new QLabel("Sharpness algorithm");
    m_comboSharpnessAlgo = new QComboBox();
    // so the size gets updated when we populate the values later
    m_comboSharpnessAlgo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);

    autoFocusLayout->addWidget(labelSharpnessAlgo, 2, 0, Qt::AlignLeft);
    autoFocusLayout->addWidget(m_comboSharpnessAlgo, 2, 1, Qt::AlignLeft);

    // Hysteresis slider
    auto labelHysteresisSlider = new QLabel("Hysteresis");
    m_hysteresisSlider = new QSlider(Qt::Orientation::Horizontal);

    m_hysteresisValueEdit = new QLineEdit("NA");

    autoFocusLayout->addWidget(labelHysteresisSlider, 3, 0, Qt::AlignLeft);
    autoFocusLayout->addWidget(m_hysteresisSlider, 3, 1, Qt::Alignment());
    autoFocusLayout->addWidget(m_hysteresisValueEdit, 3, 2, Qt::AlignRight);

    // Range sliders
    auto labelSearchRangeMinSlider = new QLabel("Search Range Min");
    m_searchRangeMinSlider = new QSlider(Qt::Orientation::Horizontal);
    m_searchRangeMinSlider->setSingleStep(1);
    m_searchRangeMinSlider->setMinimum(0);
    m_searchRangeMinSlider->setMaximum(1023);

    m_searchRangeMinValueEdit = new QLineEdit("NA");

    autoFocusLayout->addWidget(labelSearchRangeMinSlider, 4, 0, Qt::AlignLeft);
    autoFocusLayout->addWidget(m_searchRangeMinSlider, 4, 1, Qt::Alignment());
    autoFocusLayout->addWidget(m_searchRangeMinValueEdit, 4, 2, Qt::AlignRight);

    auto labelSearchRangeMaxSlider = new QLabel("Search Range Max");
    m_searchRangeMaxSlider = new QSlider(Qt::Orientation::Horizontal);
    m_searchRangeMaxSlider->setSingleStep(1);
    m_searchRangeMaxSlider->setMinimum(0);
    m_searchRangeMaxSlider->setMaximum(1023);

    m_searchRangeMaxValueEdit = new QLineEdit("NA");

    autoFocusLayout->addWidget(labelSearchRangeMaxSlider, 5, 0, Qt::AlignLeft);
    autoFocusLayout->addWidget(m_searchRangeMaxSlider, 5, 1, Qt::Alignment());
    autoFocusLayout->addWidget(m_searchRangeMaxValueEdit, 5, 2, Qt::AlignRight);

    auto roiLayout = new QGridLayout();
    roiLayout->setSpacing(6);
    // Region of Interest (ROI) sliders
    auto labelROIOffsetXSlider = new QLabel("Offset X");
    m_roiOffsetXSlider = new QSlider(Qt::Orientation::Horizontal);

    m_roiOffsetXValueEdit = new QLineEdit("NA");

    roiLayout->addWidget(labelROIOffsetXSlider, 0, 0, Qt::AlignLeft);
    roiLayout->addWidget(m_roiOffsetXSlider, 0, 1, Qt::Alignment());
    roiLayout->addWidget(m_roiOffsetXValueEdit, 0, 2, Qt::AlignRight);

    auto labelROIOffsetYSlider = new QLabel("Offset Y");
    m_roiOffsetYSlider = new QSlider(Qt::Orientation::Horizontal);

    m_roiOffsetYValueEdit = new QLineEdit("NA");

    roiLayout->addWidget(labelROIOffsetYSlider, 1, 0, Qt::AlignLeft);
    roiLayout->addWidget(m_roiOffsetYSlider, 1, 1, Qt::Alignment());
    roiLayout->addWidget(m_roiOffsetYValueEdit, 1, 2, Qt::AlignRight);

    auto labelROIWidthSlider = new QLabel("Width");
    m_roiWidthSlider = new QSlider(Qt::Orientation::Horizontal);

    m_roiWidthValueEdit = new QLineEdit("NA");

    roiLayout->addWidget(labelROIWidthSlider, 2, 0, Qt::AlignLeft);
    roiLayout->addWidget(m_roiWidthSlider, 2, 1, Qt::Alignment());
    roiLayout->addWidget(m_roiWidthValueEdit, 2, 2, Qt::AlignRight);

    auto labelROIHeightSlider = new QLabel("Height");
    m_roiHeightSlider = new QSlider(Qt::Orientation::Horizontal);

    m_roiHeightValueEdit = new QLineEdit("NA");

    roiLayout->addWidget(labelROIHeightSlider, 3, 0, Qt::AlignLeft);
    roiLayout->addWidget(m_roiHeightSlider, 3, 1, Qt::Alignment());
    roiLayout->addWidget(m_roiHeightValueEdit, 3, 2, Qt::AlignRight);

    auto roiButtonsLine = new QHBoxLayout(nullptr);
    roiButtonsLine->setSpacing(12);
    auto showROIButton = new QPushButton("Toggle ROI visibility!");
    showROIButton->setToolTip("Toggle showing the currently active region of interest (ROI) for the autofocus feature");
    showROIButton->setCheckable(true);
    auto drawROIButton = new QPushButton("Toggle drawing ROI!");
    drawROIButton->setToolTip("When active click and drag on the image to drawn a region of interest (ROI)");
    drawROIButton->setCheckable(true);

    connect(showROIButton, &QPushButton::clicked, this, &MainWindow::OnToggleROI);
    connect(drawROIButton, &QPushButton::clicked, this, &MainWindow::OnDrawROI);

    roiButtonsLine->addWidget(showROIButton, Qt::AlignLeft);
    roiButtonsLine->addWidget(drawROIButton, Qt::AlignLeft);

    auto roiGroup = new QGroupBox("Autofocus ROI", m_controlsWidget);
    auto roiGroupLayout = new QVBoxLayout(roiGroup);
    roiGroupLayout->addLayout(roiLayout, 1);
    roiGroupLayout->addLayout(roiButtonsLine, 1);

    auto generalGroup = new QGroupBox("General", m_controlsWidget);
    generalGroup->setLayout(generalGridLayout);

    auto autoFocusGroup = new QGroupBox("Autofocus", m_controlsWidget);
    autoFocusGroup->setLayout(autoFocusLayout);

    vLayoutControl->addWidget(generalGroup, 0);
    vLayoutControl->addWidget(autoFocusGroup, 0);
    vLayoutControl->addWidget(roiGroup, 0);
    vLayoutControl->addStretch(1);

    m_controlsWidget->show();
}

void MainWindow::InitializeControls()
{
    try
    {
        auto exposureRange = m_backEnd.GetExposureRange();
        m_exposureSlider->setSingleStep(static_cast<int>(exposureRange.inc));
        // Slider in us
        m_exposureSlider->setMinimum(static_cast<int>(std::ceil(exposureRange.min)));
        m_exposureSlider->setMaximum(static_cast<int>(exposureRange.max));
    }
    catch (const std::exception&)
    {
        m_exposureSlider->setEnabled(false);
    }

    // Determine the current ExposureTime
    double exposureInUs;
    try
    {
        exposureInUs = m_backEnd.GetExposureInUs();
        m_exposureSlider->setValue(static_cast<int>(exposureInUs));
        // Exposure value is displayed in ms instead of us
        m_exposureValueEdit->setText(QString::number(exposureInUs / US_TO_MS_DENOMINATOR));
    }
    catch (const std::exception&)
    {
        m_exposureSlider->setEnabled(false);
    }

    // Make sure GainSelector is set to a mode where the gain is applied to all color channels
    bool gainAvailable = m_backEnd.SetGainSelectorToAll();
    if (gainAvailable)
    {
        try
        {
            auto gainRange = m_backEnd.GetGainRange();
            m_gainSlider->setSingleStep(static_cast<int>(gainRange.inc));
            m_gainSlider->setMinimum(static_cast<int>(gainRange.min * 1000.0));
            m_gainSlider->setMaximum(static_cast<int>(gainRange.max * 1000.0));
            // Get current gain
            double gain = m_backEnd.GetGain();
            m_gainSlider->setValue(static_cast<int>(gain * 1000.0));

            m_gainValueEdit->setText(QString::number(gain));
        }
        catch (const std::exception&)
        {
            m_gainSlider->setEnabled(false);
            m_gainValueEdit->setEnabled(false);
        }
    }
    else
    {
        m_gainSlider->setEnabled(false);
        m_gainValueEdit->setEnabled(false);
    }

    try
    {
        auto fpsRange = m_backEnd.GetFrameRateRange();
        m_fpsSlider->setSingleStep(static_cast<int>(fpsRange.inc * 1000.0));
        m_fpsSlider->setMinimum(std::ceil(fpsRange.min * 1000.0));
        m_fpsSlider->setMaximum(static_cast<int>(fpsRange.max * 1000.0));

        // Get current fps
        double fps = m_backEnd.GetFrameRate();
        m_fpsSlider->setValue(static_cast<int>(fps * 1000.0));

        m_fpsValueEdit->setText(QString::number(fps));
    }
    catch (const std::exception&)
    {
        m_fpsSlider->setEnabled(false);
        m_fpsValueEdit->setEnabled(false);
    }

    if (!m_backEnd.IsAutoFocusModeSupported())
    {
        // otherwise deactivate the widgets
        m_comboFocusMode->setEnabled(false);
    }
    else
    {
        // Install callback that is invoked when the autofocus is done adjusting
        auto success = m_backEnd.SetAutoControllerFinishedCallback([this]() { this->OnAutoFocusFinished(); });
        if (!success)
        {
            m_comboFocusMode->setEnabled(false);
        }

        m_backEnd.SetAutoControllerProcessingCallback([this](int focusValue, int sharpnessValue) {
            qInfo() << "focus value: " << focusValue << " - " << "sharpness value:" << sharpnessValue;
         });
    }

    for (const auto& algo : m_backEnd.GetSearchAlgorithms())
    {
        switch (algo)
        {
        case PEAK_AFL_CONTROLLER_ALGORITHM_AUTO:
            m_comboSearchAlgo->addItem("Auto");
            break;
        case PEAK_AFL_CONTROLLER_ALGORITHM_GOLDEN_RATIO_SEARCH:
            m_comboSearchAlgo->addItem("Golden Ratio Search");
            break;
        case PEAK_AFL_CONTROLLER_ALGORITHM_HILL_CLIMBING_SEARCH:
            m_comboSearchAlgo->addItem("Hill Climbing Search");
            break;
        case PEAK_AFL_CONTROLLER_ALGORITHM_GLOBAL_SEARCH:
            m_comboSearchAlgo->addItem("Global Search");
            break;
        case PEAK_AFL_CONTROLLER_ALGORITHM_FULL_SCAN:
            m_comboSearchAlgo->addItem("Full Scan");
            break;
        }
    }

    for (const auto& algo : m_backEnd.GetSharpnessAlgorithms())
    {
        switch (algo)
        {
        case PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_AUTO:
            m_comboSharpnessAlgo->addItem("Auto");
            break;
        case PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_TENENGRAD:
            m_comboSharpnessAlgo->addItem("Tenengrad");
            break;
        case PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_SOBEL:
            m_comboSharpnessAlgo->addItem("Sobel");
            break;
        case PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_MEAN_SCORE:
            m_comboSharpnessAlgo->addItem("Mean Score");
            break;
        case PEAK_AFL_CONTROLLER_SHARPNESS_ALGORITHM_HISTOGRAM_VARIANCE:
            m_comboSharpnessAlgo->addItem("Histogram Variance");
            break;
        }
    }

    if (m_backEnd.IsHysteresisSupported())
    {
        auto hysteresisRange = m_backEnd.GetHysteresisRange();
        m_hysteresisSlider->setSingleStep(hysteresisRange.inc);
        m_hysteresisSlider->setMinimum(hysteresisRange.min);
        m_hysteresisSlider->setMaximum(hysteresisRange.max);

        // Get current hysteresis
        auto hysteresis = m_backEnd.GetHysteresis();
        m_hysteresisSlider->setValue(hysteresis);
        m_hysteresisValueEdit->setText(QString::number(hysteresis));
    }
    else
    {
        qDebug() << "Hysteresis not supported!";
        m_hysteresisSlider->setEnabled(false);
        m_hysteresisValueEdit->setEnabled(false);
    }

    if (m_backEnd.IsSearchRangeSupported())
    {
        try
        {
            // Get the minimum, maximum and increment for the search range
            auto range = m_backEnd.GetSearchRangeRange();
            m_searchRangeMinSlider->setRange(static_cast<int>(range.min), static_cast<int>(range.max));
            m_searchRangeMinSlider->setSingleStep(static_cast<int>(range.inc));
            m_searchRangeMaxSlider->setRange(static_cast<int>(range.min), static_cast<int>(range.max));
            m_searchRangeMaxSlider->setSingleStep(static_cast<int>(range.inc));

            // Get the current search range
            peak_afl_controller_limit limit = m_backEnd.GetSearchRangeLimit();
            m_searchRangeMinSlider->setValue(limit.min);
            m_searchRangeMinValueEdit->setText(QString::number(limit.min));
            m_searchRangeMaxSlider->setValue(limit.max);
            m_searchRangeMaxValueEdit->setText(QString::number(limit.max));
        }
        catch (const std::exception& e)
        {
            qDebug() << "Exception thrown when setting up autofocus range sliders:" << e.what();
            m_searchRangeMinSlider->setEnabled(false);
            m_searchRangeMinValueEdit->setEnabled(false);
            m_searchRangeMaxSlider->setEnabled(false);
            m_searchRangeMaxValueEdit->setEnabled(false);
        }
    }
    else
    {
        qDebug() << "SearchRange not supported!";
        m_searchRangeMinSlider->setEnabled(false);
        m_searchRangeMinValueEdit->setEnabled(false);
        m_searchRangeMaxSlider->setEnabled(false);
        m_searchRangeMaxValueEdit->setEnabled(false);
    }

    if (m_backEnd.IsAutoFocusROISupported())
    {
        try
        {
            // NOTE: Assuming there is a default ROI if ROI is supported
            peak_afl_rectangle roi = m_backEnd.GetROI();
            m_roiOffsetXSlider->setValue(static_cast<int>(roi.x));
            m_roiOffsetXValueEdit->setText(QString::number(roi.x));
            m_roiOffsetYSlider->setValue(static_cast<int>(roi.y));
            m_roiOffsetYValueEdit->setText(QString::number(roi.y));
            m_roiWidthSlider->setValue(static_cast<int>(roi.width));
            m_roiWidthValueEdit->setText(QString::number(roi.width));
            m_roiHeightSlider->setValue(static_cast<int>(roi.height));
            m_roiHeightValueEdit->setText(QString::number(roi.height));
            m_ROI = roi;

            // Once acquisition of images has started the image settings like offset and w/h can't be changed anymore
            // until the acquisition stops
            // So we can query the X/Y/Width/Height here
            m_imageSize.width = m_backEnd.GetImageWidth();
            m_imageSize.height = m_backEnd.GetImageHeight();
            // Query minimum supported size for the ROI that the autofocus can use
            peak_afl_size roiMinSize = m_backEnd.GetROIMinSize();
            m_roiMinSize = roiMinSize;

            m_roiOffsetXSlider->setSingleStep(1);
            m_roiOffsetXSlider->setMinimum(0);
            m_roiOffsetXSlider->setMaximum(static_cast<int>(m_imageSize.width - roiMinSize.width));
            m_roiOffsetYSlider->setSingleStep(1);
            m_roiOffsetYSlider->setMinimum(0);
            m_roiOffsetYSlider->setMaximum(static_cast<int>(m_imageSize.height - roiMinSize.height));

            m_roiWidthSlider->setSingleStep(1);
            m_roiWidthSlider->setMinimum(static_cast<int>(roiMinSize.width));
            m_roiWidthSlider->setMaximum(static_cast<int>(m_imageSize.width - roiMinSize.width));
            m_roiHeightSlider->setSingleStep(1);
            m_roiHeightSlider->setMinimum(static_cast<int>(roiMinSize.height));
            m_roiHeightSlider->setMaximum(static_cast<int>(m_imageSize.height - roiMinSize.height));
        }
        catch (const std::exception&)
        {
            m_roiOffsetXSlider->setEnabled(false);
            m_roiOffsetXValueEdit->setEnabled(false);
            m_roiOffsetYSlider->setEnabled(false);
            m_roiOffsetYValueEdit->setEnabled(false);
            m_roiWidthSlider->setEnabled(false);
            m_roiWidthValueEdit->setEnabled(false);
            m_roiHeightSlider->setEnabled(false);
            m_roiHeightValueEdit->setEnabled(false);
        }
    }
    else
    {
        qDebug() << "Setting ROI not supported!";
        m_roiOffsetXSlider->setEnabled(false);
        m_roiOffsetXValueEdit->setEnabled(false);
        m_roiOffsetYSlider->setEnabled(false);
        m_roiOffsetYValueEdit->setEnabled(false);
        m_roiWidthSlider->setEnabled(false);
        m_roiWidthValueEdit->setEnabled(false);
        m_roiHeightSlider->setEnabled(false);
        m_roiHeightValueEdit->setEnabled(false);
    }
}

void MainWindow::ConnectControls()
{
    // use signal editingFinished, which does not fire when the text is set programmatically
    connect(m_exposureValueEdit, &QLineEdit::editingFinished, this,
        [&]() { OnExposureChanged(static_cast<int>(m_exposureValueEdit->text().toFloat() * US_TO_MS_DENOMINATOR)); });
    connect(m_exposureSlider, &QSlider::valueChanged, this,
        [&](int value) { m_exposureValueEdit->setText(QString::number(value / US_TO_MS_DENOMINATOR)); });
    connect(m_exposureSlider, &QSlider::valueChanged, this, &MainWindow::OnExposureChanged);

    connect(m_gainSlider, &QSlider::valueChanged, this, &MainWindow::OnGainChanged);
    // use signal editingFinished, which does not fire when the text is set programmatically
    // Gain slider ranges from 1k-20k, but the value is displayed as 1/1000
    connect(m_gainValueEdit, &QLineEdit::editingFinished, this,
        [&]() { OnGainChanged(static_cast<int>(m_gainValueEdit->text().toFloat() * 1000.0f)); });
    connect(m_gainSlider, &QSlider::valueChanged, this,
        [&](int value) { m_gainValueEdit->setText(QString::number(static_cast<float>(value) / 1000.0f)); });

    connect(m_fpsSlider, &QSlider::valueChanged, this, &MainWindow::OnFrameRateChanged);
    // use signal editingFinished, which does not fire when the text is set programmatically
    // FPS slider ranges from 1k-10k, but the value is displayed as 1/1000
    connect(m_fpsValueEdit, &QLineEdit::editingFinished, this,
        [&]() { OnFrameRateChanged(static_cast<int>(m_fpsValueEdit->text().toFloat() * 1000.0f)); });
    connect(m_fpsSlider, &QSlider::valueChanged, this,
        [&](int value) { m_fpsValueEdit->setText(QString::number(static_cast<float>(value) / 1000.0f)); });

    connect(m_comboFocusMode, &QComboBox::currentTextChanged, this, &MainWindow::OnFocusModeChange);
    connect(m_comboSearchAlgo, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::OnSearchAlgoChanged);

    connect(m_comboSharpnessAlgo, qOverload<int>(&QComboBox::currentIndexChanged), this,
        &MainWindow::OnSharpnessAlgoChanged);

    connect(m_hysteresisSlider, &QSlider::valueChanged, this, &MainWindow::OnHysteresisChanged);

    // use signal editingFinished, which does not fire when the text is set programmatically
    connect(m_hysteresisValueEdit, &QLineEdit::editingFinished, this,
        [&]() { m_hysteresisSlider->setValue(m_hysteresisValueEdit->text().toInt()); });
    connect(m_hysteresisSlider, &QSlider::valueChanged, this,
        [&](int value) { m_hysteresisValueEdit->setText(QString::number(value)); });

    connect(m_searchRangeMinSlider, &QSlider::sliderReleased, this, [&]() { OnSearchRangeChanged(true); });
    // use signal editingFinished, which does not fire when the text is set programmatically
    connect(m_searchRangeMinValueEdit, &QLineEdit::editingFinished, this, [&]() {
        m_searchRangeMinSlider->setValue(m_searchRangeMinValueEdit->text().toInt());
        OnSearchRangeChanged(true);
    });
    connect(m_searchRangeMinSlider, &QSlider::valueChanged, this,
        [&](int value) { m_searchRangeMinValueEdit->setText(QString::number(value)); });

    connect(m_searchRangeMaxSlider, &QSlider::sliderReleased, this, [&]() { OnSearchRangeChanged(false); });
    // use signal editingFinished, which does not fire when the text is set programmatically
    connect(m_searchRangeMaxValueEdit, &QLineEdit::editingFinished, this, [&]() {
        m_searchRangeMaxSlider->setValue(m_searchRangeMaxValueEdit->text().toInt());
        OnSearchRangeChanged(false);
    });
    connect(m_searchRangeMaxSlider, &QSlider::valueChanged, this,
        [&](int value) { m_searchRangeMaxValueEdit->setText(QString::number(value)); });

    connect(m_roiOffsetXSlider, &QSlider::sliderReleased, this, &MainWindow::OnROIChanged);
    connect(m_roiOffsetXValueEdit, &QLineEdit::editingFinished, this, [&]() {
        m_roiOffsetXSlider->setValue(m_roiOffsetXValueEdit->text().toInt());
        OnROIChanged();
    });
    connect(m_roiOffsetXSlider, &QSlider::valueChanged, this,
        [&](int value) { m_roiOffsetXValueEdit->setText(QString::number(value)); });

    connect(m_roiOffsetYSlider, &QSlider::sliderReleased, this, &MainWindow::OnROIChanged);
    connect(m_roiOffsetYValueEdit, &QLineEdit::editingFinished, this, [&]() {
        m_roiOffsetYSlider->setValue(m_roiOffsetYValueEdit->text().toInt());
        OnROIChanged();
    });
    connect(m_roiOffsetYSlider, &QSlider::valueChanged, this,
        [&](int value) { m_roiOffsetYValueEdit->setText(QString::number(value)); });

    connect(m_roiWidthSlider, &QSlider::sliderReleased, this, &MainWindow::OnROIChanged);
    connect(m_roiWidthValueEdit, &QLineEdit::editingFinished, this, [&]() {
        m_roiWidthSlider->setValue(m_roiWidthValueEdit->text().toInt());
        OnROIChanged();
    });
    connect(m_roiWidthSlider, &QSlider::valueChanged, this,
        [&](int value) { m_roiWidthValueEdit->setText(QString::number(value)); });

    connect(m_roiHeightSlider, &QSlider::sliderReleased, this, &MainWindow::OnROIChanged);
    connect(m_roiHeightValueEdit, &QLineEdit::editingFinished, this, [&]() {
        m_roiHeightSlider->setValue(m_roiHeightValueEdit->text().toInt());
        OnROIChanged();
    });
    connect(m_roiHeightSlider, &QSlider::valueChanged, this,
        [&](int value) { m_roiHeightValueEdit->setText(QString::number(value)); });
}

void MainWindow::OnFocusModeChange(const QString& text)
{
    peak_afl_controller_automode mode;
    if (text == "Continuous")
    {
        mode = PEAK_AFL_CONTROLLER_AUTOMODE_CONTINUOUS;
    }
    else if (text == "Once")
    {
        mode = PEAK_AFL_CONTROLLER_AUTOMODE_ONCE;
    }
    else
    {
        mode = PEAK_AFL_CONTROLLER_AUTOMODE_OFF;
    }

    ChangeFocusMode(mode);
}

void MainWindow::ChangeFocusMode(peak_afl_controller_automode mode)
{
    m_backEnd.SetFocusMode(mode);
}

void MainWindow::OnAutoFocusFinished()
{
    auto current = m_comboFocusMode->currentText();
    if (current == "Once")
    {
        // Auto mode will be set to Off after the autofocus is done adjusting
        // so set the Autofocus setting accordingly
        // block signals, so it won't trigger onFocusModeChange that normally gets called when the text changes
        bool prev = m_comboFocusMode->blockSignals(true);
        m_comboFocusMode->setCurrentText("Off");
        m_comboFocusMode->blockSignals(prev);

        qInfo() << "Auto focus finished!";
    }
}

void MainWindow::OnExposureChanged(int exposureTimeInUs)
{
    // in case this came from the edit box
    if (exposureTimeInUs > m_exposureSlider->maximum())
    {
        exposureTimeInUs = m_exposureSlider->maximum();
    }
    else if (exposureTimeInUs < m_exposureSlider->minimum())
    {
        exposureTimeInUs = m_exposureSlider->minimum();
    }

    m_exposureValueEdit->setText(QString::number(static_cast<double>(exposureTimeInUs) / US_TO_MS_DENOMINATOR));
    m_exposureSlider->setValue(exposureTimeInUs);

    try
    {
        // Setting the exposure changes the maximum and might also change the current frame rate
        double fps;
        double maxFps;
        m_backEnd.SetExposure(exposureTimeInUs, fps, maxFps);
        auto prev = m_fpsSlider->blockSignals(true);
        m_fpsSlider->setValue(static_cast<int>(fps * 1000.0));
        // minimum fps is fixed (as in does not change with exposure like the maximum AquisitionFrameRate does)
        m_fpsSlider->setMaximum(static_cast<int>(maxFps * 1000.0));
        m_fpsSlider->blockSignals(prev);
        m_fpsValueEdit->setText(QString::number(fps));
    }
    catch (const std::exception& e)
    {
        QMessageBox::information(this, "ExposureTime", e.what(), QMessageBox::Ok);
    }
}

void MainWindow::OnGainChanged(int sliderValue)
{
    // in case this came from the edit box
    if (sliderValue > m_gainSlider->maximum())
    {
        sliderValue = m_gainSlider->maximum();
    }
    else if (sliderValue < m_gainSlider->minimum())
    {
        sliderValue = m_gainSlider->minimum();
    }

    float gain = static_cast<float>(sliderValue) / 1000.0f;
    m_gainValueEdit->setText(QString::number(gain));
    m_gainSlider->setValue(sliderValue);

    try
    {
        m_backEnd.SetGain(gain);
    }
    catch (const std::exception& e)
    {
        QMessageBox::information(this, "Gain", e.what(), QMessageBox::Ok);
    }
}

void MainWindow::OnFrameRateChanged(int sliderValue)
{
    // in case this came from the edit box
    if (sliderValue > m_fpsSlider->maximum())
    {
        sliderValue = m_fpsSlider->maximum();
    }
    else if (sliderValue < m_fpsSlider->minimum())
    {
        sliderValue = m_fpsSlider->minimum();
    }

    float fps = static_cast<float>(sliderValue) / 1000.0f;
    m_fpsValueEdit->setText(QString::number(fps));
    m_fpsSlider->setValue(sliderValue);

    try
    {
        m_backEnd.SetFrameRate(fps);
    }
    catch (const std::exception& e)
    {
        QMessageBox::information(this, "Frame Rate", e.what(), QMessageBox::Ok);
    }
}

void MainWindow::OnSearchAlgoChanged(int searchAlgoIndex)
{
    m_backEnd.SetSearchAlgorithm(searchAlgoIndex);
}

void MainWindow::OnSharpnessAlgoChanged(int sharpnessAlgoIndex)
{
    m_backEnd.SetSharpnessAlgorithm(sharpnessAlgoIndex);
}

void MainWindow::OnHysteresisChanged(int value)
{
    m_backEnd.SetHysteresis(value);
}

void MainWindow::OnSearchRangeChanged(bool minChanged)
{
    // max must always be >= min
    int minValue = m_searchRangeMinSlider->value();
    int maxValue = m_searchRangeMaxSlider->value();
    if (minValue > maxValue)
    {
        if (minChanged)
        {
            minValue = maxValue;
            bool prev = m_searchRangeMinSlider->blockSignals(true);
            m_searchRangeMinSlider->setValue(minValue);
            m_searchRangeMinValueEdit->setText(QString::number(minValue));
            m_searchRangeMinSlider->blockSignals(prev);
        }
        else
        {
            maxValue = minValue;
            bool prev = m_searchRangeMaxSlider->blockSignals(true);
            m_searchRangeMaxSlider->setValue(maxValue);
            m_searchRangeMaxValueEdit->setText(QString::number(maxValue));
            m_searchRangeMaxSlider->blockSignals(prev);
        }
    }
    if (m_backEnd.SetSearchRange(minValue, maxValue))
    {
        // update ValueEdit
        if (minChanged)
        {
            m_searchRangeMinValueEdit->setText(QString::number(minValue));
        }
        else
        {
            m_searchRangeMaxValueEdit->setText(QString::number(maxValue));
        }
    }
}

void MainWindow::OnROIChanged()
{
    peak_afl_rectangle roi;
    roi.x = m_roiOffsetXSlider->value();
    roi.y = m_roiOffsetYSlider->value();
    roi.width = m_roiWidthSlider->value();
    roi.height = m_roiHeightSlider->value();

    if (m_backEnd.SetROI(roi))
    {
        m_ROI = roi;

        if (m_showingROI)
        {
            OnShowROI();
        }
    }
    else
    {
        // setting roi failed
        // -> restore slider values from old m_ROI
        m_roiOffsetXSlider->setValue(static_cast<int>(m_ROI.x));
        m_roiOffsetYSlider->setValue(static_cast<int>(m_ROI.y));
        m_roiWidthSlider->setValue(static_cast<int>(m_ROI.width));
        m_roiHeightSlider->setValue(static_cast<int>(m_ROI.height));
    }

    // update text values
    // NOTE: even if the ROI was out of bounds, we still need to update the text values, since
    //       the edit might've been done using one of the text boxes
    //       -> use m_ROI instead of roi though!
    m_roiOffsetXValueEdit->setText(QString::number(m_ROI.x));
    m_roiOffsetYValueEdit->setText(QString::number(m_ROI.y));
    m_roiWidthValueEdit->setText(QString::number(m_ROI.width));
    m_roiHeightValueEdit->setText(QString::number(m_ROI.height));
}

void MainWindow::OnToggleROI()
{
    m_showingROI = !m_showingROI;
    OnShowROI();
}

void MainWindow::OnShowROI()
{
    if (m_showingROI)
    {
        // Position and size of the image that is shown in the GUI inside the QGraphicsScene
        QRectF imageRect = m_display->getImageRect();
        auto scene = m_display->scene();
        // m_ROI.roi is in image/sensor coordinates
        // so we need to normalize them
        double offsetXNormalized = static_cast<double>(m_ROI.x) / static_cast<double>(m_imageSize.width);
        double offsetYNormalized = static_cast<double>(m_ROI.y) / static_cast<double>(m_imageSize.height);
        double widthNormalized = static_cast<double>(m_ROI.width) / static_cast<double>(m_imageSize.width);
        double heightNormalized = static_cast<double>(m_ROI.height) / static_cast<double>(m_imageSize.height);

        // and then convert them to image coordinates in scene space
        double imageROIOffsetX = imageRect.x() + offsetXNormalized * imageRect.width();
        double imageROIOffsetY = imageRect.y() + offsetYNormalized * imageRect.height();
        double imageROIWidth = widthNormalized * imageRect.width();
        double imageROIHeight = heightNormalized * imageRect.height();
        QRectF imageROI(imageROIOffsetX, imageROIOffsetY, imageROIWidth, imageROIHeight);
        if (m_roiRect != nullptr)
        {
            m_roiRect->setRect(imageROI);
        }
        else
        {
            m_roiRect = scene->addRect(imageROI, QPen(QColor(0, 255, 0)));
        }
    }
    else if (m_roiRect != nullptr)
    {
        m_display->scene()->removeItem(m_roiRect);
        m_roiRect = nullptr;
    }
}

void MainWindow::OnDrawROI()
{
    m_display->toggleDrawROI();
}

void MainWindow::OnNewROI(QRectF roi)
{
    // roi is in scene coordinates
    QRectF imageRect = m_display->getImageRect();
    // width/height can be negative
    // use normalized() to get a rect with positive w/h
    roi = roi.normalized();
    // subtract image offset (inside the scene)
    // and normalize
    auto offsetXNormalized = static_cast<float>((roi.x() - imageRect.x()) / imageRect.width());
    auto offsetYNormalized = static_cast<float>((roi.y() - imageRect.y()) / imageRect.height());
    auto widthNormalized = static_cast<float>(roi.width() / imageRect.width());
    auto heightNormalized = static_cast<float>(roi.height() / imageRect.height());

    float camROIOffsetX = offsetXNormalized * static_cast<float>(m_imageSize.width);
    float camROIOffsetY = offsetYNormalized * static_cast<float>(m_imageSize.height);
    float camROIWidth = widthNormalized * static_cast<float>(m_imageSize.width);
    float camROIHeight = heightNormalized * static_cast<float>(m_imageSize.height);

    auto prev = m_roiOffsetXSlider->blockSignals(true);
    m_roiOffsetXSlider->setValue(static_cast<int>(camROIOffsetX));
    m_roiOffsetXSlider->blockSignals(prev);

    prev = m_roiOffsetYSlider->blockSignals(true);
    m_roiOffsetYSlider->setValue(static_cast<int>(camROIOffsetY));
    m_roiOffsetYSlider->blockSignals(prev);

    prev = m_roiWidthSlider->blockSignals(true);
    m_roiWidthSlider->setValue(static_cast<int>(camROIWidth));
    m_roiWidthSlider->blockSignals(prev);

    prev = m_roiHeightSlider->blockSignals(true);
    m_roiHeightSlider->setValue(static_cast<int>(camROIHeight));
    m_roiHeightSlider->blockSignals(prev);

    OnROIChanged();
}

void MainWindow::OnCounterChanged(unsigned int frameCounter, unsigned int errorCounter)
{
    // NOTE: if message box pops up during creation of the widgets, we might not have
    // created the labelInfo widget yet
    if (m_labelInfo != nullptr)
    {
        m_labelInfo->setText(QString("Frames acquired: %1, errors: %2")
                                 .arg(QString::number(frameCounter), QString::number(errorCounter)));
    }
}

void MainWindow::On_aboutQt_linkActivated(const QString& link)
{
    if (link == "#aboutQt")
    {
        QMessageBox::aboutQt(this, "About Qt");
    }
}
