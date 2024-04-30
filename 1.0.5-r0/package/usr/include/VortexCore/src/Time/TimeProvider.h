/**************************************************************************************************
* Module Name    : TimeProvider.h
*
* Description    : Implementation to access epoch and monotonic time functions.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-27  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_TIMEPROVIDER_H
#define VORTEXCORE_TIMEPROVIDER_H

#include <chrono>
#include <memory>

#include "ITimeProvider.h"


namespace vortex {

class TimeProvider : public ITimeProvider {
public:
    TimeProvider();
    double getTime() override;
    uint64_t getTimeMilliSec() override;
    double getMonotonicTime() override;
    void sleep(float sleep_time) override;
    void sleep(float sleep_time, Event *event) override;
    bool waitEvent(Event *event) override;
    bool waitEvent(Event *event, float timeout) override;
    std::unique_ptr<std::tm> getDate() override;

private:
    std::chrono::steady_clock::time_point steadyReference;
};

} /* namespace vortex */

#endif //VORTEXCORE_TIMEPROVIDER_H
