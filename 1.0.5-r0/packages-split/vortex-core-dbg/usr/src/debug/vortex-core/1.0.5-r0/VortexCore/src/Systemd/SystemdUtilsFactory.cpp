/**************************************************************************************************
* Module Name    : SystemdUtilsFactory.cpp
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

#include "SystemdUtilsFactory.h"
#ifndef VORTEX_CORE_SDK
#include "SystemdUtils.h"
#else
#include "MockedSystemdUtils.h"
#endif

namespace vortex {

std::shared_ptr<ISystemdUtils> SystemdUtilsFactory::override = nullptr;

std::shared_ptr<ISystemdUtils> SystemdUtilsFactory::getSystemdUtils() {
    if (nullptr != SystemdUtilsFactory::override) {
        return SystemdUtilsFactory::override;
    }
#ifndef VORTEX_CORE_SDK
    return std::static_pointer_cast<ISystemdUtils>(std::make_shared<SystemdUtils>());
#else
    return std::static_pointer_cast<ISystemdUtils>(std::make_shared<MockedSystemdUtils>());
#endif
}

void SystemdUtilsFactory::overrideSystemdUtils(std::shared_ptr<ISystemdUtils> utils) {
    SystemdUtilsFactory::override = std::move(utils);
}

void SystemdUtilsFactory::clearSystemdUtilsOverride() {
    SystemdUtilsFactory::override = nullptr;
}

} /* namespace vortex */
