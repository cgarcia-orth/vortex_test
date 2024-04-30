/**************************************************************************************************
* Module Name    : SystemdUtilsFactory.h
*
* Description    : Factory class to create SystemdUtils objects.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-26  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_SYSTEMDUTILSFACTORY_H
#define VORTEXCORE_SYSTEMDUTILSFACTORY_H

#include <memory>

#include "ISystemdUtils.h"

namespace vortex {

class SystemdUtilsFactory {
public:
    static std::shared_ptr<ISystemdUtils> getSystemdUtils();
    static void overrideSystemdUtils(std::shared_ptr<ISystemdUtils> utils);
    static void clearSystemdUtilsOverride();

private:
    static std::shared_ptr<ISystemdUtils> override;
};

} /* namespace vortex */

#endif //VORTEXCORE_SYSTEMDUTILSFACTORY_H
