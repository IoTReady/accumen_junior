/*!
 * \file    softwaretriggerworker.cpp
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.1.0
 *
 * \brief   The SoftwareTriggerWorker class is used in a worker thread to
 *          generate a software trigger.
 *
 * \version 1.0.0
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

#ifndef SOFTWARETRIGGERWORKER_H
#define SOFTWARETRIGGERWORKER_H

#include <peak/peak.hpp>


class SoftwareTriggerWorker
{

public:
    SoftwareTriggerWorker(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice);
    ~SoftwareTriggerWorker();

    void start();
    void stop();

    void run();

    bool isTriggerActive();
    void setTriggerActive(bool triggerActive);
    void setTriggerTypes(std::string triggerTypeStart, std::string triggerTypeEnd = "");
    void setSleepTimes(uint64_t sleep_ms, uint64_t sleep2_ms = 0);

private:
    bool m_triggerActive;
    bool m_threadStarted;
    uint64_t m_sleep_ms;
    uint64_t m_sleep2_ms;

    std::string m_triggerTypeStart;
    std::string m_triggerTypeEnd;

    std::shared_ptr<peak::core::NodeMap> m_nodeMapRemoteDevice;

    std::thread m_thread;
};

#endif // SOFTWARETRIGGERWORKER_H


