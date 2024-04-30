/**************************************************************************************************
* Module Name    : MockedDebounce.cpp
*
* Description    : Mocked Debounce implementation for testing.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#include "MockedDebounce.h"

namespace vortex {

template<typename DebounceType>
MockedDebounce<DebounceType>::MockedDebounce(DebounceType initialState) {
    aProcessCalled = false;
    aSetStateCalled = false;
    aMockedState = initialState;
    aSetDelayCalled = false;
    aMockedDelay = 0;
    aForceInternalStateCalled = false;
}

template<typename DebounceType>
void MockedDebounce<DebounceType>::process() {
    aProcessCalled = true;
}

template<typename DebounceType>
void MockedDebounce<DebounceType>::setState(DebounceType state) {
    aSetStateValue = state;
    aSetStateCalled = true;
}

template<typename DebounceType>
DebounceType MockedDebounce<DebounceType>::getState() {
    return aMockedState;
}

template<typename DebounceType>
void MockedDebounce<DebounceType>::setDelay(double delay) {
    aSetDelayValue = delay;
    aSetDelayCalled = true;
}

template<typename DebounceType>
double MockedDebounce<DebounceType>::getDelay() {
    return aMockedDelay;
}

template<typename DebounceType>
void MockedDebounce<DebounceType>::forceInternalState(DebounceType state) {
    aForceInternalStateValue = state;
    aForceInternalStateCalled = true;
}

}
