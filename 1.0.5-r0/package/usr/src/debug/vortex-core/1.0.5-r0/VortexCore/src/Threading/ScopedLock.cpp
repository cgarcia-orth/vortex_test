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

#include <pthread.h>
#include "ScopedLock.h"


namespace vortex {

ScopedLock::ScopedLock(pthread_mutex_t* pMutex) {
    aMutexPtr = pMutex;
    pthread_mutex_lock(aMutexPtr);
}
ScopedLock::~ScopedLock() {
    pthread_mutex_unlock(aMutexPtr);
}

}