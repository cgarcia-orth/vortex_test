#include <gmock/gmock.h>
#include "Logger/LoggerFactory.h"

int main(int argc, char **argv) {
    vortex::LoggerFactory::initTest();
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
