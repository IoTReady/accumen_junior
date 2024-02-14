/*!
 * \file    backend.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-03-07
 * \since   1.0.0
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

#ifndef BACKEND_H
#define BACKEND_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>
#include <cstdint>
#include <memory>

#include "acquisitionworker.h"
#include <peak/peak.hpp>

class BackEnd : public QObject
{
    Q_OBJECT

public:
    explicit BackEnd(QObject* parent = nullptr);
    ~BackEnd() override;

    bool Start();

    bool OpenDevice();
    void CloseDevice();
    bool InitializeAFL();

    int GetImageWidth() const;
    int GetImageHeight() const;

    bool IsAutoFocusModeSupported();
    bool SetFocusMode(peak_afl_controller_automode mode);

    peak::afl::Range<double> GetExposureRange();
    double GetExposureInUs();
    void SetExposure(double exposureTimeInUs, double& fpsOut, double& maxFpsOut);

    bool SetGainSelectorToAll();
    peak::afl::Range<double> GetGainRange();
    double GetGain();
    void SetGain(double gain);

    peak::afl::Range<double> GetFrameRateRange();
    double GetFrameRate();
    void SetFrameRate(double frameRate);

    void UpdateSearchAlgorithms();
    const std::vector<peak_afl_controller_algorithm>& GetSearchAlgorithms();
    void SetSearchAlgorithm(int searchAlgorithmIndex);

    void UpdateSharpnessAlgorithms();
    const std::vector<peak_afl_controller_sharpness_algorithm>& GetSharpnessAlgorithms();
    void SetSharpnessAlgorithm(int sharpnessAlgorithmIndex);

    bool IsHysteresisSupported();
    peak::afl::Range<uint8_t> GetHysteresisRange();
    uint8_t GetHysteresis();
    void SetHysteresis(uint8_t hysteresis);

    bool IsSearchRangeSupported();
    peak::afl::Range<int64_t> GetSearchRangeRange();
    peak_afl_controller_limit GetSearchRangeLimit();
    bool SetSearchRange(int min, int max);

    bool IsAutoFocusROISupported();
    peak_afl_rectangle GetROI();
    peak_afl_size GetROIMinSize();
    bool SetROI(peak_afl_rectangle roi);

    bool SetAutoControllerFinishedCallback(const peak::afl::callback::FinishedCallback& callback);
    bool SetAutoControllerProcessingCallback(const peak::afl::callback::DataProcessingCallback& callback);

    bool HasDeviceAutoFocus();
    void EnsureDeviceAutoFeaturesAreDisabled();

private:
    std::shared_ptr<peak::core::Device> m_device;
    std::shared_ptr<peak::core::DataStream> m_dataStream;
    std::shared_ptr<peak::core::NodeMap> m_nodemapRemoteDevice;

    bool m_aflInitialized = false;
    std::shared_ptr<peak::afl::Manager> m_autoFeatureManager{};
    std::shared_ptr<peak::afl::Controller> m_autoController{};

    std::vector<peak_afl_controller_algorithm> m_searchAlgorithms{};
    std::vector<peak_afl_controller_sharpness_algorithm> m_sharpnessAlgorithms{};

    AcquisitionWorker* m_acquisitionWorker{};
    QThread m_acquisitionThread;

    bool NodeIsAvailable(const std::string& node_name);
    bool NodeIsReadable(const std::string& node_name);
    bool NodeIsWriteable(const std::string& node_name);

signals:
    void ImageReceived(QImage image);
    void CounterChanged(unsigned int frameCounter, unsigned int errorCounter);
    void MessageBoxTrigger(QString messageTitle, QString messageText);
};

#endif // BACKEND_H
