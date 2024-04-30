/**************************************************************************************************
* Module Name    : IDebounce.h
*
* Description    : Interface to debounce a state.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_IDEBOUNCE_H
#define VORTEXCORE_IDEBOUNCE_H

namespace vortex {

template<typename DebounceType>
class IDebounce {
public:
    virtual ~IDebounce() = default;

    /**
     * Call this function periodically to allow the debounce to be computed.
     */
    virtual void process() = 0;

    /**
     * Set the current state to be debounced. Note that this set will not affect debounce output immediately as the
     * debounce delay has to be taken into account.
     * @param state The current state to be debounced.
     */
    virtual void setState(DebounceType state) = 0;

    /**
     * This is the debounced state.
     * @return The debounced state.
     */
    virtual DebounceType getState() = 0;

    virtual void setDelay(double delay) = 0;

    virtual double getDelay() = 0;

    /**
     * This will set the given state as the current state returned by getState and reset internal debouncing delays.
     * @param state
     */
    virtual void forceInternalState(DebounceType state) = 0;

};

} /* namespace vortex */

#endif //VORTEXCORE_IDEBOUNCE_H
