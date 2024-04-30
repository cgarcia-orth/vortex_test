/**************************************************************************************************
* Module Name    : Event.cpp
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

#include "Event.h"
#include <sys/time.h>
#include <cstdint>
#include <cmath>

namespace vortex {

Event::Event() {
    pthread_condattr_t attributes;
    pthread_condattr_init(&attributes);
    pthread_condattr_setclock(&attributes, CLOCK_MONOTONIC);
    pthread_cond_init(&cond, &attributes);
    pthread_mutex_init(&mutex, nullptr);
    pthread_condattr_destroy(&attributes);
    flag = false;
}

bool Event::isSet() {
    return flag;
}

void Event::set() {
    pthread_mutex_lock(&mutex);
    flag = true;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void Event::clear() {
    pthread_mutex_lock(&mutex);
    flag = false;
    pthread_mutex_unlock(&mutex);
}

bool Event::wait(float timeout) {
    bool result;
    pthread_mutex_lock(&mutex);
    if (!flag) {
        if (timeout > 0) {
            struct timespec timeToWait{};
            struct timespec now{};
            clock_gettime(CLOCK_MONOTONIC, &now);
            auto seconds = (__time_t)timeout;
            auto nanoSeconds = (__syscall_slong_t)((timeout - (float)seconds) * 1000000000.0f);
            seconds += now.tv_sec;
            nanoSeconds += now.tv_nsec;
            while (nanoSeconds >= 1000000000ULL) {
                seconds += 1;
                nanoSeconds -= 1000000000ULL;
            }
            timeToWait.tv_sec = seconds;
            timeToWait.tv_nsec = nanoSeconds;
            pthread_cond_timedwait(&cond, &mutex, &timeToWait);
        } else {
            pthread_cond_wait(&cond, &mutex);
        }
    }
    result = flag;
    pthread_mutex_unlock(&mutex);
    return result;
}

} /* namespace vortex */
