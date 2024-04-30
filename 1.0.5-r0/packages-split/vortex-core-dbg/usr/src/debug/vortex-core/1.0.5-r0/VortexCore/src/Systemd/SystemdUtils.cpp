/**************************************************************************************************
* Module Name    : SystemdUtils.cpp
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

#include "SystemdUtils.h"
#include <systemd/sd-daemon.h>

namespace vortex {

void SystemdUtils::feedWatchdog() {
    sd_notify(0,"WATCHDOG=1");
}

} /* namespace vortex */
