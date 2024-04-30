/**************************************************************************************************
* Module Name    : MockedDebounce.h
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

#ifndef VORTEXCORE_MOCKEDDEBOUNCE_H
#define VORTEXCORE_MOCKEDDEBOUNCE_H

#include "IDebounce.h"

namespace vortex {

template<typename DebounceType>
class MockedDebounce : public IDebounce<DebounceType> {
public:
    explicit MockedDebounce(DebounceType initialState);

    void process() override;
    void setState(DebounceType state) override;
    DebounceType getState() override;
    void setDelay(double delay) override;
    double getDelay() override;
    void forceInternalState(DebounceType state) override;

    bool aProcessCalled;
    bool aSetStateCalled;
    DebounceType aSetStateValue{};
    DebounceType aMockedState;
    bool aSetDelayCalled;
    double aSetDelayValue{};
    double aMockedDelay;
    bool aForceInternalStateCalled;
    DebounceType aForceInternalStateValue{};
};

} /* namespace vortex */

#endif //VORTEXCORE_MOCKEDDEBOUNCE_H
