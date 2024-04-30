/**************************************************************************************************
* Module Name    : ThreadedDebounce.h
*
* Description    : Threaded debounce implementation. Allows to debounce a binary state with a custom delay on a
*                  dedicated thread.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_THREADEDDEBOUNCE_H
#define VORTEXCORE_THREADEDDEBOUNCE_H

#include <memory>
#include <thread>

#include <VortexCore/includes/Event>

#include "IThreadedDebounce.h"
#include "Debounce.h"

namespace vortex {

template<typename DebounceType>
class ThreadedDebounce : public IThreadedDebounce<DebounceType>, public Debounce<DebounceType> {
public:
    ThreadedDebounce(DebounceType initialState, double delay,
                     typename IThreadedDebounce<DebounceType>::IListener *pListener);

    // IThreadedDebounce
    void stop() override;
    void setState(DebounceType state) override;
    void process() override;
    DebounceType getState() override;
    void setDelay(double delay) override;
    double getDelay() override;
    void forceInternalState(DebounceType state) override;

private:
    void threadProcess();

private:
    typename IThreadedDebounce<DebounceType>::IListener *aListenerPtr;
    bool aAlive;
    std::unique_ptr<std::thread> aThreadPtr;
    vortex::Event aSleepEvent;
};

} /* namespace vortex */

#endif //VORTEXCORE_THREADEDDEBOUNCE_H
