# CMake generated Testfile for 
# Source directory: /home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/git/test/cmake_add_subdirectory
# Build directory: /home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/build/test/cmake_add_subdirectory
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cmake_add_subdirectory_configure "/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native/usr/bin/cmake" "-G" "Ninja" "-Dnlohmann_json_source=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/git" "/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/git/test/cmake_add_subdirectory/project")
set_tests_properties(cmake_add_subdirectory_configure PROPERTIES  FIXTURES_SETUP "cmake_add_subdirectory")
add_test(cmake_add_subdirectory_build "/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native/usr/bin/cmake" "--build" ".")
set_tests_properties(cmake_add_subdirectory_build PROPERTIES  FIXTURES_REQUIRED "cmake_add_subdirectory")
