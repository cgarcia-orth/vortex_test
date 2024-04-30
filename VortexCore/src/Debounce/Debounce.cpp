/**************************************************************************************************
* Module Name    : Debounce.cpp
*
* Description    : Debounce implementation. Allows to debounce a binary state with a custom delay.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#include <string>

#include <VortexCore/includes/ScopedLock>

#include "Debounce.h"

namespace vortex {

template<typename DebounceType>
Debounce<DebounceType>::Debounce(DebounceType initialState, double delay) {
    Mutex::initialiseRecursiveMutex(&aLock);
    aTimeProviderPtr = TimeProviderFactory::getTimeProvider();
    aDelay = delay;
    aDebounceTimestamp = 0;
    // Initialise state machine
    aState = initialState;
    aNewState = aState;
    aProcessState = eProcessState::STABLE_STATE;
}

template<typename DebounceType>
void Debounce<DebounceType>::process() {
    ScopedLock lock(&aLock);
    // Check for state transitions
    double current_time = aTimeProviderPtr->getMonotonicTime();
    switch (aProcessState) {
        case eProcessState::TRANSITION_TO_NEW_STATE:
            // Check if debounce timeout is passed
            if ((current_time - aDebounceTimestamp) >= aDelay) {
                // Transition complete
                aProcessState = eProcessState::STABLE_STATE;
                aState = aNewState;
            }
            break;
        default:
            // Nothing to do
            break;
    }
}

template<typename DebounceType>
void Debounce<DebounceType>::setState(DebounceType state) {
    ScopedLock lock(&aLock);
    switch (aProcessState) {
        case eProcessState::STABLE_STATE:
            // We only start a transition if we detect a presence
            if (aState != state) {
                aNewState = state;
                aDebounceTimestamp = aTimeProviderPtr->getMonotonicTime();
                aProcessState = eProcessState::TRANSITION_TO_NEW_STATE;
            }
            break;
        case eProcessState::TRANSITION_TO_NEW_STATE:
            // If there is no more presence, we go back to not present
            if (aState == state) {
                aProcessState = eProcessState::STABLE_STATE;
            } else if (aNewState != state) {
                aNewState = state;
                aDebounceTimestamp = aTimeProviderPtr->getMonotonicTime();
            }
            break;
        default:
            // Should never get here
            aProcessState = eProcessState::STABLE_STATE;
            break;
    }
}

template<typename DebounceType>
DebounceType Debounce<DebounceType>::getState() {
    ScopedLock lock(&aLock);
    return aState;
}

template<typename DebounceType>
void Debounce<DebounceType>::setDelay(double delay) {
    ScopedLock lock(&aLock);
    aDelay = delay;
}

template<typename DebounceType>
double Debounce<DebounceType>::getDelay() {
    ScopedLock lock(&aLock);
    return aDelay;
}

template<typename DebounceType>
void Debounce<DebounceType>::forceInternalState(DebounceType state) {
    ScopedLock lock(&aLock);
    aState = state;
    aNewState = aState;
    aProcessState = eProcessState::STABLE_STATE;
}

template class Debounce<bool>;
template class Debounce<std::string>;

} /* namespace vortex */
