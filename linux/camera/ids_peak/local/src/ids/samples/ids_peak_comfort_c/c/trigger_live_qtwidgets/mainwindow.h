/*!
 * \file    mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "acquisitionworker.h"
#include "display.h"

#include "backend.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

#include <cstdint>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    bool hasError() const;

private:
    QWidget* m_centralWidget{};
    QWidget* m_controlsWidget{};

    Display* m_display{};
    QLabel* m_labelCameraInfo{};
    QLabel* m_labelInfo{};
    QVBoxLayout* m_layout{};
    QGridLayout* m_layoutTriggerControls{};
    QGridLayout* m_layoutFreerunControls{};
    QFrame* m_frameTriggerControls{};
    QFrame* m_frameFreerunControls{};

    AcquisitionWorker* m_acquisitionWorker{};
    QThread m_acquisitionThread{};

    QPushButton* m_buttonStartStop{};
    QPushButton* m_buttonTrigger{};

    QLabel* m_labelExposureTime{};
    QSlider* m_sliderExposureTime{};
    QLineEdit* m_editExposureTime{};

    QLabel* m_labelFrameRate{};
    QSlider* m_sliderFrameRate{};
    QLineEdit* m_editFrameRate{};

    QLabel* m_labelTriggerDelay{};
    QSlider* m_sliderTriggerDelay{};
    QLineEdit* m_editTriggerDelay{};

    QLabel* m_labelTriggerDivider{};
    QSlider* m_sliderTriggerDivider{};
    QLineEdit* m_editTriggerDivider{};

    QLabel* m_labelTriggerEdge{};
    QComboBox* m_comboTriggerEdge{};

    QRadioButton* m_radioFreerun{};
    QRadioButton* m_radioSoftwareTrigger{};
    QRadioButton* m_radioHardwareTrigger{};
    QRadioButton* m_radioHardwareTriggerGPIO1{};
    QRadioButton* m_radioHardwareTriggerGPIO2{};

    QButtonGroup* m_radioButtonGroup{};

    QGroupBox* m_generalGroupBox{};
    QGridLayout* m_generalLayout{};

    QGroupBox* m_acquisitionGroupBox{};
    QVBoxLayout* m_acquisitionLayout{};

    QGroupBox* m_optionsGroupBox{};
    QVBoxLayout* m_optionsLayout{};

    peak_camera_handle m_hCam{};

    double m_triggerDelayMin{};
    double m_triggerDelayMax{};
    double m_triggerDelayInc{};

    uint32_t m_triggerDividerMin{};
    uint32_t m_triggerDividerMax{};
    uint32_t m_triggerDividerInc{};

    int m_triggerEdge{};

    double m_exposureTime{};
    double m_exposureTimeMin{};
    double m_exposureTimeMax{};
    double m_exposureTimeInc{};

    double m_frameRate{};
    double m_frameRateMin{};
    double m_frameRateMax{};
    double m_frameRateInc{};
    double m_frameRateLast{};

    bool m_hasError{};
    bool m_blockErrorMessages{};

    void ComposeMainWindow();
    void CreateControls();

    void UpdateTriggerChannel();

    void UpdateTriggerDelayControl();
    void UpdateTriggerDividerControl();
    void UpdateTriggerEdgeControl();
    void UpdateExposureTimeControl();
    void UpdateFrameRateControl();

    void UpdateTriggerExecuteControl();

    void CreateAcquisitionWorkerThread();
    void StartAcquisition();
    void StopAcquisition();

    static void EmitErrorSignal(const char* message, int status, void* errorCallbackContext);

public slots:
    void HandleLibraryError(QString message, int status) const;
    void HandleErrorAndQuit(QString message) const;

    void OnButtonStopStart();
    static void OnButtonTrigger();
    void OnRadioFreerun(bool state);
    void OnRadioSoftwareTrigger(bool state);
    void OnRadioHardwareTrigger(bool state);
    void OnRadioHardwareTriggerGPIO1(bool state);
    void OnRadioHardwareTriggerGPIO2(bool state);

    void OnSliderTriggerDelay();
    void OnEditTriggerDelay();

    void OnSliderTriggerDivider();
    void OnEditTriggerDivider();

    void OnSliderExposureTime();
    void OnEditExposureTime();

    void OnSliderFrameRate();
    void OnEditFrameRate();

    void OnComboTriggerEdge(const QString& text);

    void UpdateControls();
    void OnCounterUpdated(unsigned int frameCounter, unsigned int errorCounter);
    void OnAboutQtLinkActivated(const QString& link);

signals:
    void libraryError(QString message, int status);
};

#endif // MAINWINDOW_H
