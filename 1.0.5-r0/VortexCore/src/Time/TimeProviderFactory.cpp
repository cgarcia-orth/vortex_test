/**************************************************************************************************
* Module Name    : TimeProviderFactory.cpp
*
* Description    : Factory class to create TimeProvider objects.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-27  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#include "TimeProviderFactory.h"
#include "TimeProvider.h"

namespace vortex {

std::shared_ptr<ITimeProvider> TimeProviderFactory::override = nullptr;

std::shared_ptr<ITimeProvider> TimeProviderFactory::getTimeProvider() {
    if (nullptr != TimeProviderFactory::override) {
        return TimeProviderFactory::override;
    }
    return std::static_pointer_cast<ITimeProvider>(std::make_shared<TimeProvider>());
}

void TimeProviderFactory::overrideTimeProvider(std::shared_ptr<ITimeProvider> provider) {
    TimeProviderFactory::override = std::move(provider);
}

void TimeProviderFactory::clearTimeProviderOverride() {
    TimeProviderFactory::override = nullptr;
}

} /* namespace vortex */
