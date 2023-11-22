/*!
 * \file    softwaretriggerworker.h
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

#include "softwaretriggerworker.h"
#include <iostream>
#include <thread>
#include <math.h>

SoftwareTriggerWorker::SoftwareTriggerWorker(std::shared_ptr<peak::core::NodeMap> nodeMapRemoteDevice)
{
    m_triggerActive = true;
    m_threadStarted = false;
    m_sleep_ms = 0;
    m_sleep2_ms = 0;

    m_triggerTypeStart = "";
    m_triggerTypeEnd = "";

    m_nodeMapRemoteDevice = nodeMapRemoteDevice;
}

SoftwareTriggerWorker::~SoftwareTriggerWorker()
{}

void SoftwareTriggerWorker::start()
{
    try
    {
        m_thread = std::thread(&SoftwareTriggerWorker::run, this);
        m_threadStarted = true;
    }
    catch (...)
    {
    }
}

void SoftwareTriggerWorker::stop()
{
    setTriggerActive(false);
    if (m_threadStarted)
    {
        while (!m_thread.joinable())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        m_thread.join();
    }
}

void SoftwareTriggerWorker::run()
{
    while (m_triggerActive)
    {
        try
        {
            // do the start trigger
            if (m_triggerTypeStart == "Counter0")
            {
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("CounterSelector")
                    ->SetCurrentEntry("Counter0");
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("CounterReset")->Execute();
            }
            else if (m_triggerTypeStart == "Timer0")
            {
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("TimerReset")->Execute();
            }
            else
            {
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(m_triggerTypeStart);
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("TriggerSoftware")->Execute();
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("TriggerSoftware")
                    ->WaitUntilDone();
            }

            // indicate the start trigger by printing a '.'
            std::cout << ". ";

            // wait for sleep_ms, but at least for two frames before proceeding
            auto const frameRate =
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::FloatNode>("AcquisitionFrameRate")->Value();
            auto const frameTime_ms = static_cast<uint64_t>(std::ceil((1.0 / frameRate) * 1000.0)) * 2;
            if (m_sleep_ms == 0)
            {
                // random exposure time (ms) for trigger case 'e'
                uint64_t exposureTime_ms = rand() % 100 + 1;
                std::this_thread::sleep_for(std::chrono::milliseconds(std::max(frameTime_ms, exposureTime_ms)));
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(std::max(m_sleep_ms, frameTime_ms)));
            }

            // if an end trigger is defined, do the end trigger (trigger cases 'a' or 'e')
            if (!m_triggerTypeEnd.empty())
            {
                // in case of AcquisitionEnd also set triggerEnabled to false to stop the image processing loop in the
                // main thread
                if (m_triggerTypeEnd == "AcquisitionEnd")
                {
                    m_triggerActive = false;


                    // allow the image processing loop in main thread to terminate
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }

                // indicate the end trigger by printing an 'x'
                std::cout << "x ";
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::EnumerationNode>("TriggerSelector")
                    ->SetCurrentEntry(m_triggerTypeEnd);
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("TriggerSoftware")->Execute();
                m_nodeMapRemoteDevice->FindNode<peak::core::nodes::CommandNode>("TriggerSoftware")
                    ->WaitUntilDone();
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "EXCEPTION: " << e.what() << std::endl;
        }

        // wait before proceeding (trigger case 'e')
        if (m_sleep2_ms != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep2_ms));
        }
    }
    return;
}

bool SoftwareTriggerWorker::isTriggerActive()
{
    return m_triggerActive;
}

void SoftwareTriggerWorker::setTriggerActive(bool triggerActive)
{
    m_triggerActive = triggerActive;
}

void SoftwareTriggerWorker::setTriggerTypes(std::string triggerTypeStart, std::string triggerTypeEnd)
{
    m_triggerTypeStart = triggerTypeStart;
    m_triggerTypeEnd = triggerTypeEnd;
}

void SoftwareTriggerWorker::setSleepTimes(uint64_t sleep_ms, uint64_t sleep2_ms)
{
    m_sleep_ms = sleep_ms;
    m_sleep2_ms = sleep2_ms;
}
