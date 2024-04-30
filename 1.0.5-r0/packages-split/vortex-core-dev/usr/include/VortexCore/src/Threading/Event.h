/**************************************************************************************************
* Module Name    : Event.h
*
* Description    : Implementation of a threading event, similar to python.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-27  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_EVENT_H
#define VORTEXCORE_EVENT_H

#include "pthread.h"

namespace vortex {

class Event {
public:
    Event();
    bool isSet();
    void set();
    void clear();
    bool wait(float timeout=0);
private:
    pthread_cond_t cond{};
    pthread_mutex_t mutex{};
    bool flag;
};

} /* namespace vortex */

#endif //VORTEXCORE_EVENT_H
