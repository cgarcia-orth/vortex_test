/**************************************************************************************************
* Module Name    : TestThreadedDebounce.cpp
*
* Description    : Threaded debounce implementation tests.
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
#include "VortexCore/src/Debounce/ThreadedDebounce.h"
#include "Test/TestUtils/TestUtils.h"

using namespace vortex;

class TestThreadedDebounce : public testing::Test, public IThreadedDebounce<bool>::IListener {
public:
    void onNewState(bool state) override;

protected:
    static const bool INITIAL_STATE = true;
    constexpr static const double INITIAL_DELAY = 0.5f;

    void SetUp() override;
    void TearDown() override;

    std::shared_ptr<MockedTimeProvider> aMockedTimeProviderPtr;
    std::unique_ptr<ThreadedDebounce<bool>> aThreadedDebouncePtr;

    bool aListenerCalled{};
    bool aState{};
};

const bool TestThreadedDebounce::INITIAL_STATE;
constexpr const double TestThreadedDebounce::INITIAL_DELAY;

void TestThreadedDebounce::SetUp() {
    aListenerCalled = false;
    // Override time provider
    aMockedTimeProviderPtr = std::make_shared<MockedTimeProvider>();
    TimeProviderFactory::overrideTimeProvider(aMockedTimeProviderPtr);
    // Create debounce
    aThreadedDebouncePtr = std::unique_ptr<ThreadedDebounce<bool>>(
            new ThreadedDebounce<bool>(TestThreadedDebounce::INITIAL_STATE, TestThreadedDebounce::INITIAL_DELAY, this));
}

void TestThreadedDebounce::TearDown() {
    aThreadedDebouncePtr->stop();
    // Clear overrides
    TimeProviderFactory::clearTimeProviderOverride();
}

void TestThreadedDebounce::onNewState(bool state) {
    aState = state;
    aListenerCalled = true;
}

TEST_F(TestThreadedDebounce, Debounce) { // NOLINT(cert-err58-cpp)
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    // Test that transitions shorter than test time are rejected
    aThreadedDebouncePtr->setState(false);
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += TestThreadedDebounce::INITIAL_DELAY - 0.001f;
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aThreadedDebouncePtr->setState(true);
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += TestThreadedDebounce::INITIAL_DELAY + 0.001f;
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    // Test a proper transition
    aThreadedDebouncePtr->setState(false);
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += TestThreadedDebounce::INITIAL_DELAY - 0.001f;
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += 0.002f;
    ASSERT_TRUE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 5.0f));
    ASSERT_EQ(aState, false);
    aListenerCalled = false;
    // Test that transitions shorter than test time are rejected
    aThreadedDebouncePtr->setState(true);
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += TestThreadedDebounce::INITIAL_DELAY - 0.001f;
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aThreadedDebouncePtr->setState(false);
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += TestThreadedDebounce::INITIAL_DELAY + 0.001f;
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    // Test a proper transition
    aThreadedDebouncePtr->setState(true);
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += TestThreadedDebounce::INITIAL_DELAY - 0.001f;
    ASSERT_FALSE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 0.05f));
    aMockedTimeProviderPtr->aMonotonicTime += 0.002f;
    ASSERT_TRUE(TestUtils::waitForCondition([&]() {
        return aListenerCalled;
    }, 5.0f));
    ASSERT_EQ(aState, true);
}
