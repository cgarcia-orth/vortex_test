/**************************************************************************************************
* Module Name    : DebounceFactory.h
*
* Description    : Factory class to create Debounce objects.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_DEBOUNCEFACTORY_H
#define VORTEXCORE_DEBOUNCEFACTORY_H

#include <memory>

#include "IDebounce.h"
#include "IThreadedDebounce.h"

namespace vortex {

class DebounceFactory {
public:
    template<typename DebounceType>
    static std::shared_ptr<IDebounce<DebounceType>> getDebounce(DebounceType initialState, double delay);

    template<typename DebounceType>
    static void overrideDebounce(std::shared_ptr<IDebounce<DebounceType>> override);

    static void clearDebounceOverride();

    template<typename DebounceType>
    static std::shared_ptr<IThreadedDebounce<DebounceType>> getThreadedDebounce(
            DebounceType initialState, double delay, typename IThreadedDebounce<DebounceType>::IListener *pListener);

    template<typename DebounceType>
    static void overrideThreadedDebounce(std::shared_ptr<IThreadedDebounce<DebounceType>> override);

    static void clearThreadedDebounceOverride();

private:
    static void *aDebounceOverride;
    static void *aThreadedDebounceOverride;
};

} /* namespace vortex */

#endif //VORTEXCORE_DEBOUNCEFACTORY_H
