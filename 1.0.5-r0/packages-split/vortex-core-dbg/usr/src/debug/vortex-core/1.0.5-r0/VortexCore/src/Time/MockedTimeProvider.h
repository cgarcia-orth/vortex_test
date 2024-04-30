/**************************************************************************************************
* Module Name    : MockedTimeProvider.h
*
* Description    : Mock implementation of time provider.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-10-01  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_MOCKEDTIMEPROVIDER_H
#define VORTEXCORE_MOCKEDTIMEPROVIDER_H

#include <memory>

#include "ITimeProvider.h"

namespace vortex {

class MockedTimeProvider : public ITimeProvider {
public:
    MockedTimeProvider() = default;

    double getTime() override;
    uint64_t getTimeMilliSec() override;
    double getMonotonicTime() override;
    void sleep(float sleep_time) override;
    void sleep(float sleep_time, Event *event) override;
    bool waitEvent(Event *event) override;
    bool waitEvent(Event *event, float timeout) override;
    std::unique_ptr<std::tm> getDate() override;

    double aMockedTime{};
    uint64_t aMockedTimeMs{};
    double aMonotonicTime{};
    std::tm aMockedDate{};

};

} /* namespace vortex */

#endif //VORTEXCORE_MOCKEDTIMEPROVIDER_H
