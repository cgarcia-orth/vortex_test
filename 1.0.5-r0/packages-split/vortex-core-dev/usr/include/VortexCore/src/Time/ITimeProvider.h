/**************************************************************************************************
* Module Name    : ITimeProvider.h
*
* Description    : Interface to access epoch and monotonic time functions.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-27  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_ITIMEPROVIDER_H
#define VORTEXCORE_ITIMEPROVIDER_H

#include <cstdint>
#include <ctime>
#include <memory>

#include "../Threading/Event.h"

namespace vortex {

class ITimeProvider {
public:
    virtual ~ITimeProvider()=default;
    virtual double getTime()=0;
    virtual uint64_t getTimeMilliSec() = 0;
    virtual double getMonotonicTime() = 0;
    virtual void sleep(float sleep_time) = 0;
    virtual void sleep(float sleep_time, Event *event) = 0;
    virtual bool waitEvent(Event *event) = 0;
    virtual bool waitEvent(Event *event, float timeout) = 0;
    virtual std::unique_ptr<std::tm> getDate() = 0;
};

} /* namespace vortex */

#endif //VORTEXCORE_ITIMEPROVIDER_H
