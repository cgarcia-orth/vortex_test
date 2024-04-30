/**************************************************************************************************
* Module Name    : IThreadedDebounce.h
*
* Description    : Interface to debounce a state with a dedicated thread.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_ITHREADEDDEBOUNCE_H
#define VORTEXCORE_ITHREADEDDEBOUNCE_H

#include "IDebounce.h"

namespace vortex {

template<typename DebounceType>
class IThreadedDebounce : public IDebounce<DebounceType> {
public:
    class IListener {
    public:
        virtual ~IListener() = default;

        virtual void onNewState(DebounceType state) = 0;
    };

public:
    virtual void stop() = 0;

protected:
    void process() override = 0;
};

} /* namespace vortex */

#endif //VORTEXCORE_ITHREADEDDEBOUNCE_H
