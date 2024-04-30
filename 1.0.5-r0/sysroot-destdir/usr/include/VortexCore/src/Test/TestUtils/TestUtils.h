/**************************************************************************************************
* Module Name    : TestUtils.h
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

#ifndef MAESTROPRO_TESTUTILS_H
#define MAESTROPRO_TESTUTILS_H

#include <functional>

namespace vortex {

class TestUtils {
public:
    static bool waitForCondition(const std::function<bool (void)>& comparator, float timeout);
};

} /* namespace vortex */

#endif //MAESTROPRO_TESTUTILS_H
