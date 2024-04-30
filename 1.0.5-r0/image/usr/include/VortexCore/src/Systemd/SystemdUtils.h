/**************************************************************************************************
* Module Name    : SystemdUtils.h
*
* Description    : Implementation of the systemd interface.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-26  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_SYSTEMDUTILS_H
#define VORTEXCORE_SYSTEMDUTILS_H

#include "ISystemdUtils.h"

namespace vortex {

class SystemdUtils : public ISystemdUtils {
public:
    void feedWatchdog() override;
};

} /* namespace vortex */

#endif //VORTEXCORE_SYSTEMDUTILS_H
