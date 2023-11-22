/*!
 * \file    autofeatures.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2023-05-15
 * \since   1.2.0
 *
 * \version 1.2.0
 *
 * Copyright (C) 2021 - 2023, IDS Imaging Development Systems GmbH.
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

#ifndef AUTOFEATURES_H
#define AUTOFEATURES_H

#include <peak_afl/peak_afl.hpp>
#include <peak_ipl/peak_ipl.hpp>
#include <peak/peak.hpp>


#include <memory>
#include <functional>

class AutoFeatures
{
public:
    AutoFeatures(std::shared_ptr<peak::core::NodeMap> nodemap);
    ~AutoFeatures();

    void ProcessImage(const peak::ipl::Image* image);
    
    void SetExposureMode(peak_afl_controller_automode mode);
    void SetGainMode(peak_afl_controller_automode mode);
    void SetWhiteBalanceMode(peak_afl_controller_automode mode);
    void SetSkipFrames(int skipFrames);
    void Reset();

    void RegisterExposureCallback(std::function<void(void)> callback);
    void RegisterGainCallback(std::function<void(void)> callback);
    void RegisterWhiteBalanceCallback(std::function<void(void)> callback);

private:
    void CreateAutoManager();
    void CreateAutoControllers();

    std::shared_ptr<peak::core::NodeMap> m_nodemapRemoteDevice{};
    peak::ipl::Gain m_gainControllerIPL{};

    std::unique_ptr<peak::afl::Manager> m_autoFeaturesManager{};
    std::shared_ptr<peak::afl::Controller> m_autoBrightnessController{};
    std::shared_ptr<peak::afl::Controller> m_autoWhiteBalanceController{};

    std::function<void(void)> m_exposureFinishedCallback;
    std::function<void(void)> m_gainFinishedCallback;
    std::function<void(void)> m_whiteBalanceFinishedCallback;
};

#endif // AUTOFEATURES_H
