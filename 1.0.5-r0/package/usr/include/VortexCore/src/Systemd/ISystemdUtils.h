/**************************************************************************************************
* Module Name    : SystemdUtilsInterface.h
*
* Description    : Interface definition to use systemd functions.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-20  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_ISYSTEMDUTILS_H
#define VORTEXCORE_ISYSTEMDUTILS_H

namespace vortex {

class ISystemdUtils {
public:
    virtual ~ISystemdUtils()=default;
    virtual void feedWatchdog() = 0;
};

} /* namespace vortex */

#endif //VORTEXCORE_ISYSTEMDUTILS_H
