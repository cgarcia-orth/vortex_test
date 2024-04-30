/**************************************************************************************************
* Module Name    : TestUtils.cpp
*
* Description    : Collection of test functions.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-10-29  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#include <VortexCore/src/Time/TimeProvider.h>

#include "TestUtils.h"

namespace vortex {

bool TestUtils::waitForCondition(const std::function<bool(void)>& comparator, float timeout) {
    TimeProvider timeProvider;
    float endTime = timeProvider.getMonotonicTime() + timeout;
    while (!comparator()) {
        if (timeProvider.getMonotonicTime() > endTime) {
            return false;
        }
    }
    return true;
}

} /* namespace vortex */
