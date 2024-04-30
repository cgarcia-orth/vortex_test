/**************************************************************************************************
* Module Name    : MockedThreadedDebounce.h
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

#ifndef VORTEXCORE_MOCKEDTHREADEDDEBOUNCE_H
#define VORTEXCORE_MOCKEDTHREADEDDEBOUNCE_H

#include "IThreadedDebounce.h"

namespace vortex {

template<typename DebounceType>
class MockedThreadedDebounce : public IThreadedDebounce<DebounceType> {
public:
    explicit MockedThreadedDebounce(DebounceType initialState);

    void setState(DebounceType state) override;
    DebounceType getState() override;
    void setDelay(double delay) override;
    double getDelay() override;
    void stop() override;
    void forceInternalState(DebounceType state) override;

    bool aSetStateCalled;
    DebounceType aSetStateValue{};
    DebounceType aMockedState;
    bool aSetDelayCalled;
    double aSetDelayValue{};
    double aMockedDelay;
    bool aStopCalled;
    bool aForceInternalStateCalled;
    DebounceType aForceInternalStateValue{};
};

} /* namespace vortex */

#endif //VORTEXCORE_MOCKEDTHREADEDDEBOUNCE_H
