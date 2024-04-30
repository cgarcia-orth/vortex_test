# CMake generated Testfile for 
# Source directory: /home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/git/test/cmake_import_minver
# Build directory: /home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/build/test/cmake_import_minver
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cmake_import_minver_configure "/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native/usr/bin/cmake" "-G" "Ninja" "-Dnlohmann_json_DIR=/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/build" "/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/git/test/cmake_import_minver/project")
set_tests_properties(cmake_import_minver_configure PROPERTIES  FIXTURES_SETUP "cmake_import_minver")
add_test(cmake_import_minver_build "/home/vortex/build/Vortex_SourceCode/yocto/build_var-som-mx6/tmp/work/armv7at2hf-neon-fslc-linux-gnueabi/libjson/3.7.0-r0/recipe-sysroot-native/usr/bin/cmake" "--build" ".")
set_tests_properties(cmake_import_minver_build PROPERTIES  FIXTURES_REQUIRED "cmake_import_minver")
