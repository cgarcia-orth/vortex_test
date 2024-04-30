/**************************************************************************************************
* Module Name    : Mutex.cpp
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

#include "Mutex.h"

namespace vortex {

void Mutex::initialiseRecursiveMutex(pthread_mutex_t *mutex) {
    pthread_mutexattr_t mutexAttributes;
    pthread_mutexattr_init(&mutexAttributes);
    pthread_mutexattr_settype(&mutexAttributes, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mutex, &mutexAttributes);
    pthread_mutexattr_destroy(&mutexAttributes);
}

} /* namespace vortex */
