cmake_minimum_required (VERSION 3.1)
project ("VortexCore" C CXX)

set(CMAKE_CXX_STANDARD 11)

# For debugging
set( CMAKE_VERBOSE_MAKEFILE true )

# Make sure toolchain name is set
if (NOT VORTEX_TOOLCHAIN_NAME)
    set(VORTEX_TOOLCHAIN_NAME "remote")
endif ()

# Make sure sysroot is set
if (VORTEX_TOOLCHAIN_NAME STREQUAL "remote")
    if (NOT CMAKE_SYSROOT)
        set(CMAKE_SYSROOT $ENV{CMAKE_ROOT_DIR})
    endif()
endif()

# Compiler options

# Force no optimisation
if (VORTEX_TOOLCHAIN_NAME STREQUAL "remote")
    string(REPLACE "-O2" "-O" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REPLACE "-O2" "-O" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_C_FLAGS -O0;-g3;-Wall)
endif()

# Use pthread
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Use no-psabi - remove warning in jsonlib
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")

# Set include paths
set(LOCAL_INCLUDE_PATH   "../" "../includes" )  # Where built & installed libraries are

include_directories( ${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/Logger
                     ${LOCAL_INCLUDE_PATH}
                     ${CMAKE_SYSROOT}/include
                     ${CMAKE_SYSROOT}/usr/include
                   )


# Shared libraries
if (VORTEX_TOOLCHAIN_NAME STREQUAL "remote")
    find_library(LIB_STDCPP             libstdc++.so)
else()
    set(CMAKE_AR /usr/bin/gcc-ar)
    set(CMAKE_RANLIB /usr/bin/gcc-ranlib)

    find_library(LIB_STDCPP             libstdc++.so PATHS /usr/lib/x86_64-linux-gnu/ NO_DEFAULT_PATH)
    # For some reason, when installing the lib by hand, CMAKE can't find the lib. I am out of time for this so I will
    # hard code the path in this case
    if (NOT LIB_STDCPP)
        set(LIB_STDCPP /usr/lib/x86_64-linux-gnu/libstdc++.so.6)
    endif()
endif()

set(VORTEX_LINUX_SHARED_LIBS ${LIB_STDCPP})

file(GLOB_RECURSE LOGGER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Logger/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Logger/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Logger/*.h)
file(GLOB_RECURSE SYSTEMD_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Systemd/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Systemd/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Systemd/*.h)
file(GLOB_RECURSE THREADING_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Threading/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Threading/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Threading/*.h)
file(GLOB_RECURSE JSONCOM_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/JsonCommunication/*.c ${CMAKE_CURRENT_SOURCE_DIR}/JsonCommunication/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/JsonCommunication/*.h)
file(GLOB_RECURSE TIME_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Time/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Time/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Time/*.h)
file(GLOB_RECURSE DEBOUNCE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Debounce/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Debounce/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Debounce/*.h)
file(GLOB_RECURSE TEST_UTILS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestUtils/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestUtils/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestUtils/*.h)

set(VORTEX_SOURCE_FILES ${LOGGER_SRCS} ${SYSTEMD_SRCS} ${THREADING_SRCS} ${TIME_SRCS} ${JSONCOM_SRCS} ${DEBOUNCE_SRCS} ${TEST_UTILS_SRCS})

# Check if we are building a version of the lib for the SDK
if (NOT CMAKE_VORTEX_CORE_NATIVESDK)
    if ($ENV{CMAKE_VORTEX_CORE_NATIVESDK})
        set(CMAKE_VORTEX_CORE_NATIVESDK $ENV{CMAKE_VORTEX_CORE_NATIVESDK})
    elseif(NOT VORTEX_TOOLCHAIN_NAME STREQUAL "remote")
        set(CMAKE_VORTEX_CORE_NATIVESDK "true")
    else()
        set(CMAKE_VORTEX_CORE_NATIVESDK "false")
    endif()
endif()
message("CMAKE_VORTEX_CORE_NATIVESDK: ${CMAKE_VORTEX_CORE_NATIVESDK}")
if (${CMAKE_VORTEX_CORE_NATIVESDK} STREQUAL "true")
    add_definitions(-DVORTEX_CORE_SDK)
    # Remove concrete implementation of systemd utils since it will be needed to dev and is not possible to add in a
    # native SDK.
    list(REMOVE_ITEM VORTEX_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Systemd/SystemdUtils.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Systemd/SystemdUtils.h)
endif()

message("Source files: ${VORTEX_SOURCE_FILES}")

## Build application
add_library(VortexCore SHARED ${VORTEX_SOURCE_FILES})
add_library(VortexCoreStatic STATIC ${VORTEX_SOURCE_FILES})

target_link_libraries(VortexCore ${VORTEX_LINUX_SHARED_LIBS})
target_link_libraries(VortexCoreStatic ${VORTEX_LINUX_SHARED_LIBS})

set_target_properties(VortexCoreStatic PROPERTIES OUTPUT_NAME VortexCore)

# Add install instructions for local build
if (VORTEX_TOOLCHAIN_NAME STREQUAL "local")
    install(TARGETS VortexCore DESTINATION /usr/lib)
    install(DIRECTORY ../includes DESTINATION /usr/include/VortexCore)
    install(DIRECTORY ../ DESTINATION /usr/include/VortexCore FILES_MATCHING PATTERN "*.h")
endif()
# Tests
if (VORTEX_TOOLCHAIN_NAME)
    # Bypass SDK for include files of VortexCore itself during tests.
    include_directories( "../../" )
    if (${VORTEX_TOOLCHAIN_NAME} STREQUAL "nativesdk")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3")

        find_library(GTEST_LIB libgtest.a)
        find_library(GMOCK_LIB libgmock.a)

        file(GLOB_RECURSE TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.h)

        add_executable(VortexCoreTests Test/main.cpp ${TEST_SRCS} ${LOGGER_SRCS} ${THREADING_SRCS} ${TIME_SRCS} ${JSONCOM_SRCS} ${DEBOUNCE_SRCS})
        target_link_libraries(VortexCoreTests ${GTEST_LIB} ${GMOCK_LIB})
    endif()
endif()
