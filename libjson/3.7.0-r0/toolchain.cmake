# CMake system name must be something like "Linux".
# This is important for cross-compiling.

set( CMAKE_SYSTEM_NAME Linux )
set( CMAKE_SYSTEM_PROCESSOR arm )
set( CMAKE_C_COMPILER arm-fslc-linux-gnueabi-gcc )
set( CMAKE_CXX_COMPILER arm-fslc-linux-gnueabi-g++ )
set( CMAKE_ASM_COMPILER arm-fslc-linux-gnueabi-gcc )
set( CMAKE_AR arm-fslc-linux-gnueabi-ar CACHE FILEPATH "Archiver" )
set( CMAKE_C_FLAGS " -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot  -O2 -pipe -g -feliminate-unused-debug-types -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0=/usr/src/debug/libjson/3.7.0-r0 -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot= -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native=   -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot" CACHE STRING "CFLAGS" )
set( CMAKE_CXX_FLAGS " -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot  -O2 -pipe -g -feliminate-unused-debug-types -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0=/usr/src/debug/libjson/3.7.0-r0 -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot= -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native=  -fvisibility-inlines-hidden  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot" CACHE STRING "CXXFLAGS" )
set( CMAKE_ASM_FLAGS " -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot  -O2 -pipe -g -feliminate-unused-debug-types -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0=/usr/src/debug/libjson/3.7.0-r0 -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot= -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native=   -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot" CACHE STRING "ASM FLAGS" )
set( CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CFLAGS for release" )
set( CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CXXFLAGS for release" )
set( CMAKE_ASM_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional ASM FLAGS for release" )
set( CMAKE_C_LINK_FLAGS " -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot  -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed -fstack-protector-strong -Wl,-z,relro,-z,now" CACHE STRING "LDFLAGS" )
set( CMAKE_CXX_LINK_FLAGS " -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot  -O2 -pipe -g -feliminate-unused-debug-types -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0=/usr/src/debug/libjson/3.7.0-r0 -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot= -fdebug-prefix-map=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native=  -fvisibility-inlines-hidden  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security  --sysroot=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed -fstack-protector-strong -Wl,-z,relro,-z,now" CACHE STRING "LDFLAGS" )

# only search in the paths provided so cmake doesnt pick
# up libraries and tools from the native build machine
set( CMAKE_FIND_ROOT_PATH /home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot /home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native    )
set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

# Use qt.conf settings
set( ENV{QT_CONF_PATH} /home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/qt.conf )

# We need to set the rpath to the correct directory as cmake does not provide any
# directory as rpath by default
set( CMAKE_INSTALL_RPATH  )

# Use native cmake modules
list(APPEND CMAKE_MODULE_PATH "/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot/usr/share/cmake/Modules/")

# add for non /usr/lib libdir, e.g. /usr/lib64
set( CMAKE_LIBRARY_PATH /usr/lib /lib)

