/**************************************************************************************************
* Module Name    : Debounce.h
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

#ifndef VORTEXCORE_DEBOUNCE_H
#define VORTEXCORE_DEBOUNCE_H

#include <VortexCore/includes/TimeProviderFactory>
#include <VortexCore/includes/Mutex>

#include "IDebounce.h"

namespace vortex {

template<typename DebounceType>
class Debounce : public IDebounce<DebounceType> {
public:
    enum class eProcessState {
        STABLE_STATE,
        TRANSITION_TO_NEW_STATE
    };

public:
    Debounce(DebounceType initialState, double delay);

    // IDebounce
    void process() override;
    void setState(DebounceType state) override;
    DebounceType getState() override;
    void setDelay(double delay) override;
    double getDelay() override;
    void forceInternalState(DebounceType state) override;

protected:
    std::shared_ptr<ITimeProvider> aTimeProviderPtr;

private:
    DebounceType aState;
    DebounceType aNewState;
    eProcessState aProcessState;
    double aDelay;
    double aDebounceTimestamp;
    pthread_mutex_t aLock{};
};

} /* namespace vortex */

#endif //VORTEXCORE_DEBOUNCE_H
