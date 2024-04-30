/**************************************************************************************************
* Module Name    : DebounceFactory.cpp
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

#include <utility>

#include "DebounceFactory.h"
#include "Debounce.h"
#include "ThreadedDebounce.h"

namespace vortex {

void *DebounceFactory::aDebounceOverride = nullptr;
void *DebounceFactory::aThreadedDebounceOverride = nullptr;

template<typename DebounceType>
std::shared_ptr<IDebounce<DebounceType>> DebounceFactory::getDebounce(DebounceType initialState, double delay) {
    if (nullptr != aDebounceOverride) {
        return *static_cast<std::shared_ptr<IDebounce<DebounceType>>*>(aDebounceOverride);
    }
    return std::make_shared<Debounce<DebounceType>>(initialState, delay);
}

template std::shared_ptr<IDebounce<bool>> DebounceFactory::getDebounce<bool>(bool initialState, double delay);
template std::shared_ptr<IDebounce<std::string>> DebounceFactory::getDebounce<std::string>(std::string initialState, double delay);

template<typename DebounceType>
void DebounceFactory::overrideDebounce(std::shared_ptr<IDebounce<DebounceType>> override) {
    aDebounceOverride = &override;
}

void DebounceFactory::clearDebounceOverride() {
    aDebounceOverride = nullptr;
}

template<typename DebounceType>
std::shared_ptr<IThreadedDebounce<DebounceType>> DebounceFactory::getThreadedDebounce(
        DebounceType initialState, double delay, typename IThreadedDebounce<DebounceType>::IListener *pListener) {
    if (nullptr != aThreadedDebounceOverride) {
        return aThreadedDebounceOverride;
    }
    return std::make_shared<ThreadedDebounce>(initialState, delay, pListener);
}

template<typename DebounceType>
void DebounceFactory::overrideThreadedDebounce(std::shared_ptr<IThreadedDebounce<DebounceType>> override) {
    aThreadedDebounceOverride = &override;
}

template void DebounceFactory::overrideThreadedDebounce<bool>(std::shared_ptr<IThreadedDebounce<bool>> override);
template void DebounceFactory::overrideThreadedDebounce<std::string>(std::shared_ptr<IThreadedDebounce<std::string>> override);

void DebounceFactory::clearThreadedDebounceOverride() {
    aThreadedDebounceOverride = nullptr;
}

}
