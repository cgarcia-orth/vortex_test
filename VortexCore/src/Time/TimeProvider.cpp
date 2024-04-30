/**************************************************************************************************
* Module Name    : TimeProvider.cpp
*
* Description    : Implementation to access epoch and monotonic time functions.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-08-27  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#include <thread>
#include <cstring>

#include "TimeProvider.h"


namespace vortex {

TimeProvider::TimeProvider() {
    steadyReference = std::chrono::steady_clock::now();
}

double TimeProvider::getTime() {
    return  std::chrono::duration_cast<std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t TimeProvider::getTimeMilliSec() {
    return  std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}

double TimeProvider::getMonotonicTime() {
    std::chrono::duration<double> timestamp = std::chrono::steady_clock::now() - steadyReference;
    return timestamp.count();
}

void TimeProvider::sleep(float sleep_time) {
    sleep(sleep_time, nullptr);
}

void TimeProvider::sleep(float sleep_time, Event *event) {
    if (nullptr == event) {
        std::this_thread::sleep_for(std::chrono::duration<float>(sleep_time));
    } else {
        if (sleep_time > 0) {
            event->wait(sleep_time);
        }
    }
}

bool TimeProvider::waitEvent(Event *event) {
    return waitEvent(event, 0);
}

bool TimeProvider::waitEvent(Event *event, float timeout) {
    return event->wait(timeout);
}

std::unique_ptr<std::tm> TimeProvider::getDate() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    std::unique_ptr<std::tm> pRet = std::unique_ptr<std::tm> (new std::tm());
    std::memcpy(pRet.get(), gmtime(&tt), sizeof(std::tm));
    return std::move(pRet);
}

} /* namespace vortex */
