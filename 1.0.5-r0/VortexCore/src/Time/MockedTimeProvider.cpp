/**************************************************************************************************
* Module Name    : MockedTimeProvider.h
*
* Description    : Mock implementation of time provider.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-10-01  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/
#include <cstring>

#include <cstring>
#include "MockedTimeProvider.h"

double vortex::MockedTimeProvider::getTime() {
    return aMockedTime;
}

uint64_t vortex::MockedTimeProvider::getTimeMilliSec() {
    return aMockedTimeMs;
}

double vortex::MockedTimeProvider::getMonotonicTime() {
    return aMonotonicTime;
}

void vortex::MockedTimeProvider::sleep(float sleep_time) {

}

void vortex::MockedTimeProvider::sleep(float sleep_time, vortex::Event *event) {

}

bool vortex::MockedTimeProvider::waitEvent(vortex::Event *event) {
    return false;
}

bool vortex::MockedTimeProvider::waitEvent(vortex::Event *event, float timeout) {
    return false;
}

std::unique_ptr<std::tm> vortex::MockedTimeProvider::getDate() {
    std::unique_ptr<std::tm> pRet = std::unique_ptr<std::tm> (new std::tm());
    std::memcpy(pRet.get(), &aMockedDate, sizeof(std::tm));
    return std::move(pRet);
}
