/**************************************************************************************************
* Module Name    : Mutex.h
*
* Description    : Utils functions for pthread mutex.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-09-17  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_MUTEX_H
#define VORTEXCORE_MUTEX_H

#include <pthread.h>

namespace vortex {

class Mutex {
public:
    static void initialiseRecursiveMutex(pthread_mutex_t *mutex);
};



} /* namespace vortex */

#endif //VORTEXCORE_MUTEX_H
