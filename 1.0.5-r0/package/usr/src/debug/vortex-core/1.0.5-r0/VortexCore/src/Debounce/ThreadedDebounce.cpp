/**************************************************************************************************
* Module Name    : ThreadedDebounce.cpp
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

#include "ThreadedDebounce.h"

namespace vortex {

template<typename DebounceType>
ThreadedDebounce<DebounceType>::ThreadedDebounce(DebounceType initialState, double delay,
                                                 typename IThreadedDebounce<DebounceType>::IListener *pListener) :
                                                 Debounce<DebounceType>(initialState, delay) {
    aListenerPtr = pListener;
    aAlive = true;
    aThreadPtr = std::unique_ptr<std::thread>(new std::thread(&ThreadedDebounce::threadProcess, this));
}

template<typename DebounceType>
void ThreadedDebounce<DebounceType>::threadProcess() {
    while (aAlive) {
        DebounceType previousState = getState();
        process();
        DebounceType newState = getState();
        if (previousState != newState) {
            if (nullptr != aListenerPtr) {
                aListenerPtr->onNewState(newState);
            }
        }
        this->aTimeProviderPtr->sleep(getDelay(), &aSleepEvent);
        aSleepEvent.clear();
    }
}

template<typename DebounceType>
void ThreadedDebounce<DebounceType>::stop() {
    aAlive = false;
    aSleepEvent.set();
    aThreadPtr->join();
    aThreadPtr = nullptr;
}

template<typename DebounceType>
void ThreadedDebounce<DebounceType>::setState(DebounceType state) {
    Debounce<DebounceType>::setState(state);
    aSleepEvent.set();
}

template<typename DebounceType>
void ThreadedDebounce<DebounceType>::process() {
    Debounce<DebounceType>::process();
}

template<typename DebounceType>
DebounceType ThreadedDebounce<DebounceType>::getState() {
    return Debounce<DebounceType>::getState();
}

template<typename DebounceType>
void ThreadedDebounce<DebounceType>::setDelay(double delay) {
    Debounce<DebounceType>::setDelay(delay);
}

template<typename DebounceType>
double ThreadedDebounce<DebounceType>::getDelay() {
    return Debounce<DebounceType>::getDelay();
}

template<typename DebounceType>
void ThreadedDebounce<DebounceType>::forceInternalState(DebounceType state) {
    Debounce<DebounceType>::forceInternalState(state);
}

template class ThreadedDebounce<bool>;
template class ThreadedDebounce<std::string>;

} /* namespace vortex */
