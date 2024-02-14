/*!
 * \file    mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "acquisitionworker.h"
#include "backend.h"
#include "display.h"

#include <peak/peak.hpp>

#include <QComboBox>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

#include <cstdint>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

    bool HasError() const;

private:
    BackEnd m_backEnd{};

    bool m_hasError = true;

    QWidget* m_CentralWidget{};
    QWidget* m_ControlWidget{};

    Display* m_display{};
    QLabel* m_labelInfo{};
    QVBoxLayout* m_layout{};

    QComboBox* m_comboFocusMode{};

    QSlider* m_exposureSlider{};
    QLineEdit* m_exposureValueEdit{};

    QSlider* m_gainSlider{};
    QLineEdit* m_gainValueEdit{};

    QSlider* m_fpsSlider{};
    QLineEdit* m_fpsValueEdit{};

    QComboBox* m_comboSearchAlgo{};

    QComboBox* m_comboSharpnessAlgo{};

    QSlider* m_hysteresisSlider{};
    QLineEdit* m_hysteresisValueEdit{};

    QSlider* m_searchRangeMinSlider{};
    QLineEdit* m_searchRangeMinValueEdit{};
    QSlider* m_searchRangeMaxSlider{};
    QLineEdit* m_searchRangeMaxValueEdit{};

    peak_afl_size m_roiMinSize{};
    peak_afl_size m_imageSize{};
    peak_afl_rectangle m_ROI{};
    QSlider* m_roiOffsetXSlider{};
    QLineEdit* m_roiOffsetXValueEdit{};
    QSlider* m_roiOffsetYSlider{};
    QLineEdit* m_roiOffsetYValueEdit{};
    QSlider* m_roiWidthSlider{};
    QLineEdit* m_roiWidthValueEdit{};
    QSlider* m_roiHeightSlider{};
    QLineEdit* m_roiHeightValueEdit{};

    QGraphicsRectItem* m_roiRect{};
    bool m_showingROI = false;

    void CreateStatusBar();
    void CreateControls();
    void InitializeControls();
    void ConnectControls();

    void ChangeFocusMode(peak_afl_controller_automode);

public slots:
    static void ShowMessageBox(const QString& messageTitle, const QString& messageText);
    void OnFocusModeChange(const QString& text);
    void OnAutoFocusFinished();
    void OnCounterChanged(unsigned int frameCounter, unsigned int errorCounter);
    void On_aboutQt_linkActivated(const QString& link);
    void OnExposureChanged(int exposureTimeInUs);
    void OnGainChanged(int value);
    void OnFrameRateChanged(int value);
    void OnSharpnessAlgoChanged(int index);
    void OnSearchAlgoChanged(int index);
    void OnHysteresisChanged(int value);
    void OnSearchRangeChanged(bool minChanged);
    void OnROIChanged();
    void OnToggleROI();
    void OnShowROI();
    void OnDrawROI();
    void OnNewROI(QRectF roi);
};

static void AutoFocusFinishedCallback(void* mainWindowInstance);

#endif // MAINWINDOW_H
