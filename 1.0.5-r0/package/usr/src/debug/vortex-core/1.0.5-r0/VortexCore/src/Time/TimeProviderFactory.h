/**************************************************************************************************
* Module Name    : TimeProviderFactory.h
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

#ifndef VORTEXCORE_TIMEPROVIDERFACTORY_H
#define VORTEXCORE_TIMEPROVIDERFACTORY_H

#include <memory>
#include "ITimeProvider.h"

namespace vortex {

class TimeProviderFactory {
public:
    static std::shared_ptr<ITimeProvider> getTimeProvider();
    static void overrideTimeProvider(std::shared_ptr<ITimeProvider> provider);
    static void clearTimeProviderOverride();

private:
    static std::shared_ptr<ITimeProvider> override;
};

} /* namespace vortex */

#endif //VORTEXCORE_TIMEPROVIDERFACTORY_H
