/**************************************************************************************************
* Module Name    : TestDebounce.cpp
*
* Description    : Debounce implementation tests.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-11-06  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/

#include <gmock/gmock.h>

#include "VortexCore/includes/TimeProviderFactory"
#include "VortexCore/src/Time/MockedTimeProvider.h"
#include "VortexCore/src/Debounce/DebounceFactory.h"

using namespace vortex;

class TestDebounce : public testing::Test {
protected:
    static const bool INITIAL_STATE = true;
    constexpr static const double INITIAL_DELAY = 0.5f;

    void SetUp() override;
    void TearDown() override;

    std::shared_ptr<MockedTimeProvider> aMockedTimeProviderPtr;
    std::shared_ptr<IDebounce<bool>> aDebouncePtr;
};

const bool TestDebounce::INITIAL_STATE;
constexpr const double TestDebounce::INITIAL_DELAY;

void TestDebounce::SetUp() {
    // Override time provider
    aMockedTimeProviderPtr = std::make_shared<MockedTimeProvider>();
    TimeProviderFactory::overrideTimeProvider(aMockedTimeProviderPtr);
    // Create debounce
    aDebouncePtr = DebounceFactory::getDebounce<bool>(TestDebounce::INITIAL_STATE, TestDebounce::INITIAL_DELAY);
}

void TestDebounce::TearDown() {
    // Clear overrides
    TimeProviderFactory::clearTimeProviderOverride();
}

TEST_F(TestDebounce, Init) { // NOLINT(cert-err58-cpp)
    // Test that debounce time is initialised and initial state is properly handled.
    ASSERT_EQ(aDebouncePtr->getState(), TestDebounce::INITIAL_STATE);
    ASSERT_EQ(aDebouncePtr->getDelay(), TestDebounce::INITIAL_DELAY);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), TestDebounce::INITIAL_STATE);
    ASSERT_EQ(aDebouncePtr->getDelay(), TestDebounce::INITIAL_DELAY);
}

TEST_F(TestDebounce, Debounce) { // NOLINT(cert-err58-cpp)
    // Set debounce time
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    // Test that transitions shorter than test time are rejected
    aDebouncePtr->setState(false);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY - 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aDebouncePtr->setState(true);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY + 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    // Test a proper transition
    aDebouncePtr->setState(false);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY - 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aMockedTimeProviderPtr->aMonotonicTime += 0.002f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
    // Test that transitions shorter than test time are rejected
    aDebouncePtr->setState(true);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY - 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
    aDebouncePtr->setState(false);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY + 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
    // Test a proper transition
    aDebouncePtr->setState(true);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY - 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
    aMockedTimeProviderPtr->aMonotonicTime += + 0.002f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
}

TEST_F(TestDebounce, NoDelay) { // NOLINT(cert-err58-cpp)
    aDebouncePtr->setDelay(0);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aDebouncePtr->setState(false);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
}

TEST_F(TestDebounce, ForceInternalState) { // NOLINT(cert-err58-cpp)
    // Set debounce time
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    // Start a proper transition
    aDebouncePtr->setState(false);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY - 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    // Force state, this should cancel the transition in progress
    aDebouncePtr->forceInternalState(true);
    aMockedTimeProviderPtr->aMonotonicTime += 0.002f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    // Test a proper transition after force state
    aDebouncePtr->setState(false);
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aMockedTimeProviderPtr->aMonotonicTime += TestDebounce::INITIAL_DELAY - 0.001f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), true);
    aMockedTimeProviderPtr->aMonotonicTime += 0.002f;
    aDebouncePtr->process();
    ASSERT_EQ(aDebouncePtr->getState(), false);
}
