Summary: vortex-core version 1.0.5-r0
Name: vortex-core
Version: 1.0.5
Release: r0
License: CLOSED
Group: base
Packager: FSL Community Team <meta-freescale@yoctoproject.org>
BuildRequires: cmake-native
BuildRequires: gtest
BuildRequires: libjson
BuildRequires: libsole
BuildRequires: ninja-native
BuildRequires: rsync-native
BuildRequires: spdlog
BuildRequires: systemd
BuildRequires: virtual/arm-fslc-linux-gnueabi-compilerlibs
BuildRequires: virtual/arm-fslc-linux-gnueabi-gcc
BuildRequires: virtual/libc
Requires: ld-linux-armhf.so.3
Requires: ld-linux-armhf.so.3(GLIBC_2.4)
Requires: libc.so.6
Requires: libc.so.6(GLIBC_2.17)
Requires: libc.so.6(GLIBC_2.28)
Requires: libc.so.6(GLIBC_2.4)
Requires: libc6 >= 2.28
Requires: libgcc1 >= 8.2.0
Requires: libgcc_s.so.1
Requires: libgcc_s.so.1(GCC_3.5)
Requires: libpthread.so.0
Requires: libpthread.so.0(GLIBC_2.12)
Requires: libpthread.so.0(GLIBC_2.4)
Requires: libstdc++.so.6
Requires: libstdc++.so.6(CXXABI_1.3)
Requires: libstdc++.so.6(CXXABI_1.3.5)
Requires: libstdc++.so.6(CXXABI_1.3.8)
Requires: libstdc++.so.6(CXXABI_ARM_1.3.3)
Requires: libstdc++.so.6(GLIBCXX_3.4)
Requires: libstdc++.so.6(GLIBCXX_3.4.11)
Requires: libstdc++.so.6(GLIBCXX_3.4.14)
Requires: libstdc++.so.6(GLIBCXX_3.4.15)
Requires: libstdc++.so.6(GLIBCXX_3.4.18)
Requires: libstdc++.so.6(GLIBCXX_3.4.19)
Requires: libstdc++.so.6(GLIBCXX_3.4.20)
Requires: libstdc++.so.6(GLIBCXX_3.4.21)
Requires: libstdc++.so.6(GLIBCXX_3.4.22)
Requires: libstdc++.so.6(GLIBCXX_3.4.9)
Requires: libstdc++6 >= 8.2.0
Requires: rtld(GNU_HASH)
Requires(post): ld-linux-armhf.so.3
Requires(post): ld-linux-armhf.so.3(GLIBC_2.4)
Requires(post): libc.so.6
Requires(post): libc.so.6(GLIBC_2.17)
Requires(post): libc.so.6(GLIBC_2.28)
Requires(post): libc.so.6(GLIBC_2.4)
Requires(post): libc6 >= 2.28
Requires(post): libgcc1 >= 8.2.0
Requires(post): libgcc_s.so.1
Requires(post): libgcc_s.so.1(GCC_3.5)
Requires(post): libpthread.so.0
Requires(post): libpthread.so.0(GLIBC_2.12)
Requires(post): libpthread.so.0(GLIBC_2.4)
Requires(post): libstdc++.so.6
Requires(post): libstdc++.so.6(CXXABI_1.3)
Requires(post): libstdc++.so.6(CXXABI_1.3.5)
Requires(post): libstdc++.so.6(CXXABI_1.3.8)
Requires(post): libstdc++.so.6(CXXABI_ARM_1.3.3)
Requires(post): libstdc++.so.6(GLIBCXX_3.4)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.11)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.14)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.15)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.18)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.19)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.20)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.21)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.22)
Requires(post): libstdc++.so.6(GLIBCXX_3.4.9)
Requires(post): libstdc++6 >= 8.2.0
Requires(post): rtld(GNU_HASH)
Provides: libVortexCore.so

%description
VortexCore

%package -n vortex-core-dbg
Summary: vortex-core version 1.0.5-r0 - Debugging files
License: CLOSED
Group: devel
Recommends: libc6-dbg
Recommends: libgcc-s-dbg
Recommends: libstdc++-dbg

%description -n vortex-core-dbg
VortexCore  This package contains ELF symbols and related sources for
debugging purposes.

%package -n vortex-core-staticdev
Summary: vortex-core version 1.0.5-r0 - Development files (Static Libraries)
License: CLOSED
Group: devel
Requires: vortex-core-dev = 1.0.5-r0

%description -n vortex-core-staticdev
VortexCore  This package contains static libraries for software
development.

%package -n vortex-core-dev
Summary: vortex-core version 1.0.5-r0 - Development files
License: CLOSED
Group: devel
Requires: vortex-core = 1.0.5-r0
Recommends: gtest-dev
Recommends: libc6-dev
Recommends: libgcc-s-dev
Recommends: libjson-dev
Recommends: libsole-dev
Recommends: libstdc++-dev
Recommends: spdlog-dev
Recommends: systemd-dev

%description -n vortex-core-dev
VortexCore  This package contains symbolic links, header files, and related
items necessary for software development.

%package -n vortex-core-doc
Summary: vortex-core version 1.0.5-r0 - Documentation files
License: CLOSED
Group: doc

%description -n vortex-core-doc
VortexCore  This package contains documentation.

%package -n vortex-core-locale
Summary: vortex-core version 1.0.5-r0
License: CLOSED
Group: base

%description -n vortex-core-locale
VortexCore

%post
# vortex-core - postinst
#!/bin/sh
set -e
if [ x"$D" = "x" ]; then
	if [ -x /sbin/ldconfig ]; then /sbin/ldconfig ; fi
fi


%files
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/lib"
"/usr/lib/libVortexCore.so.0"

%files -n vortex-core-dbg
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/lib"
%dir "/usr/src"
%dir "/usr/lib/.debug"
"/usr/lib/.debug/libVortexCore.so.0"
%dir "/usr/src/debug"
%dir "/usr/src/debug/vortex-core"
%dir "/usr/src/debug/vortex-core/1.0.5-r0"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Threading"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Logger"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/JsonCommunication"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Test"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Threading/Mutex.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Threading/Event.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Threading/ScopedLock.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Threading/Event.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Threading/ScopedLock.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Threading/Mutex.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Logger/LoggerFactory.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Logger/LoggerFactory.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/JsonCommunication/JsonCommunication.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/JsonCommunication/JsonCommunication.cpp"
%dir "/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Test/TestUtils"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Test/TestUtils/TestUtils.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Test/TestUtils/TestUtils.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time/MockedTimeProvider.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time/TimeProvider.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time/ITimeProvider.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time/TimeProviderFactory.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time/MockedTimeProvider.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time/TimeProviderFactory.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Time/TimeProvider.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd/ISystemdUtils.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd/MockedSystemdUtils.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd/SystemdUtils.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd/SystemdUtilsFactory.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd/SystemdUtils.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd/SystemdUtilsFactory.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Systemd/MockedSystemdUtils.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/IDebounce.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/ThreadedDebounce.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/ThreadedDebounce.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/DebounceFactory.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/Debounce.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/Debounce.cpp"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/IThreadedDebounce.h"
"/usr/src/debug/vortex-core/1.0.5-r0/VortexCore/src/Debounce/DebounceFactory.h"

%files -n vortex-core-staticdev
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/lib"
"/usr/lib/libVortexCore.a"

%files -n vortex-core-dev
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/include"
%dir "/usr/lib"
%dir "/usr/include/VortexCore"
%dir "/usr/include/VortexCore/src"
%dir "/usr/include/VortexCore/includes"
%dir "/usr/include/VortexCore/src/Threading"
%dir "/usr/include/VortexCore/src/Logger"
%dir "/usr/include/VortexCore/src/JsonCommunication"
%dir "/usr/include/VortexCore/src/Test"
%dir "/usr/include/VortexCore/src/Time"
%dir "/usr/include/VortexCore/src/Systemd"
%dir "/usr/include/VortexCore/src/Debounce"
"/usr/include/VortexCore/src/Threading/Event.h"
"/usr/include/VortexCore/src/Threading/ScopedLock.h"
"/usr/include/VortexCore/src/Threading/Mutex.h"
"/usr/include/VortexCore/src/Logger/LoggerFactory.h"
"/usr/include/VortexCore/src/JsonCommunication/JsonCommunication.h"
"/usr/include/VortexCore/src/JsonCommunication/IJsonMsgListener.h"
%dir "/usr/include/VortexCore/src/Test/JsonCommunication"
%dir "/usr/include/VortexCore/src/Test/TestUtils"
%dir "/usr/include/VortexCore/src/Test/Debounce"
"/usr/include/VortexCore/src/Test/TestUtils/TestUtils.h"
"/usr/include/VortexCore/src/Time/MockedTimeProvider.h"
"/usr/include/VortexCore/src/Time/ITimeProvider.h"
"/usr/include/VortexCore/src/Time/TimeProviderFactory.h"
"/usr/include/VortexCore/src/Time/TimeProvider.h"
"/usr/include/VortexCore/src/Systemd/ISystemdUtils.h"
"/usr/include/VortexCore/src/Systemd/MockedSystemdUtils.h"
"/usr/include/VortexCore/src/Systemd/SystemdUtils.h"
"/usr/include/VortexCore/src/Systemd/SystemdUtilsFactory.h"
"/usr/include/VortexCore/src/Debounce/MockedThreadedDebounce.h"
"/usr/include/VortexCore/src/Debounce/IDebounce.h"
"/usr/include/VortexCore/src/Debounce/ThreadedDebounce.h"
"/usr/include/VortexCore/src/Debounce/MockedDebounce.h"
"/usr/include/VortexCore/src/Debounce/Debounce.h"
"/usr/include/VortexCore/src/Debounce/IThreadedDebounce.h"
"/usr/include/VortexCore/src/Debounce/DebounceFactory.h"
"/usr/include/VortexCore/includes/TimeProviderFactory"
"/usr/include/VortexCore/includes/ScopedLock"
"/usr/include/VortexCore/includes/JsonCommunication"
"/usr/include/VortexCore/includes/ErrorCodes"
"/usr/include/VortexCore/includes/SystemdUtilsFactory"
"/usr/include/VortexCore/includes/Mutex"
"/usr/include/VortexCore/includes/LoggerFactory"
"/usr/include/VortexCore/includes/Debounce"
"/usr/include/VortexCore/includes/Event"
"/usr/lib/libVortexCore.so"

