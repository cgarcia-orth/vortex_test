/**************************************************************************************************
* Module Name    : ScopeLock.h
*
* Description    : Utils functions for pthread mutex scope locking
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-10-17  Olivier Battion : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_SCOPEDLOCK_H
#define VORTEXCORE_SCOPEDLOCK_H

#include <pthread.h>

namespace vortex {

class ScopedLock {
public:
    ScopedLock(pthread_mutex_t* mutex);
    ~ScopedLock();
private :
    pthread_mutex_t* aMutexPtr;

};

}

#endif // VORTEXCORE_SCOPEDLOCK_H