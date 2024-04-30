/**************************************************************************************************
* Module Name    : MockedSystemdUtils.h
*
* Description    : Mocked implementation of the systemd interface. Intended to be used for tests and the SDK.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-29  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_MOCKEDSYSTEMDUTILS_H
#define VORTEXCORE_MOCKEDSYSTEMDUTILS_H

#include "ISystemdUtils.h"

namespace vortex {

class MockedSystemdUtils : public ISystemdUtils {
public:
    void feedWatchdog() override;
};

} /* namespace vortex */

#endif //VORTEXCORE_MOCKEDSYSTEMDUTILS_H
