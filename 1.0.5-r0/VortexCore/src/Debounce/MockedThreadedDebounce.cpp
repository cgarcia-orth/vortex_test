/**************************************************************************************************
* Module Name    : MockedThreadedDebounce.cpp
*
* Description    : Mocked ThreadedDebounce implementation for testing.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#include "MockedThreadedDebounce.h"

namespace vortex {

template<typename DebounceType>
MockedThreadedDebounce<DebounceType>::MockedThreadedDebounce(DebounceType initialState) {
    aSetStateCalled = false;
    aMockedState = initialState;
    aSetDelayCalled = false;
    aMockedDelay = 0;
    aStopCalled = false;
    aForceInternalStateCalled = false;
}

template<typename DebounceType>
void MockedThreadedDebounce<DebounceType>::setState(DebounceType state) {
    aSetStateValue = state;
    aSetStateCalled = true;
}

template<typename DebounceType>
DebounceType MockedThreadedDebounce<DebounceType>::getState() {
    return aMockedState;
}

template<typename DebounceType>
void MockedThreadedDebounce<DebounceType>::setDelay(double delay) {
    aSetDelayValue = delay;
    aSetDelayCalled = true;
}

template<typename DebounceType>
double MockedThreadedDebounce<DebounceType>::getDelay() {
    return aMockedDelay;
}

template<typename DebounceType>
void MockedThreadedDebounce<DebounceType>::stop() {
    aStopCalled = true;
}

template<typename DebounceType>
void MockedThreadedDebounce<DebounceType>::forceInternalState(DebounceType state) {
    aForceInternalStateValue = state;
    aForceInternalStateCalled = true;
}

} /* namespace vortex */
