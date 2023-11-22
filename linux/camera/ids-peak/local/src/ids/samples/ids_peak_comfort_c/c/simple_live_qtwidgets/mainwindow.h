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


#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
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
    ~MainWindow() override;

    bool hasError() const;

private:
    QWidget* m_centralWidget{};
    Display* m_display{};
    QLabel* m_labelCameraInfo{};
    QLabel* m_labelInfo{};
    QVBoxLayout* m_layout{};

    AcquisitionWorker* m_acquisitionWorker{};
    QThread m_acquisitionThread{};

    QPushButton* m_buttonStartStop{};

    QLabel* m_labelExposureTime{};
    QSlider* m_sliderExposureTime{};
    QLineEdit* m_editExposureTime{};

    QLabel* m_labelFrameRate{};
    QSlider* m_sliderFrameRate{};
    QLineEdit* m_editFrameRate{};

    QCheckBox* m_checkBoxMirrorLeftRight{};
    QCheckBox* m_checkBoxMirrorUpDown{};

    peak_camera_handle m_hCam{};

    double m_exposureTimeMin{};
    double m_exposureTimeMax{};
    double m_exposureTimeInc{};

    double m_frameRate{};
    double m_frameRateMin{};
    double m_frameRateInc{};

    bool m_hasError{};
    bool m_blockErrorMessages{};

    void ComposeMainWindow();
    void CreateControls();
    void UpdateExposureTimeControl();
    void UpdateFrameRateControl();
    void UpdateMirrorControls();

    void CreateAcquisitionWorkerThread();
    void StartAcquisition();
    void StopAcquisition();

    static void EmitErrorSignal(const char* message, int status, void* errorCallbackContext);

public slots:
    void HandleLibraryError(QString message, int status) const;
    void HandleErrorAndQuit(QString message) const;

    void OnButtonStopStart();
    void OnSliderExposureTime();
    void OnEditExposureTime();
    void OnSliderFrameRate();
    void OnEditFrameRate();
    void UpdateControls();
    void OnCheckBoxMirrorLeftRight();
    void OnCheckBoxMirrorUpDown();
    void OnCounterUpdated(unsigned int frameCounter, unsigned int errorCounter);
    void OnAboutQtLinkActivated(const QString& link);

signals:
    void libraryError(QString message, int status);
};

#endif // MAINWINDOW_H
