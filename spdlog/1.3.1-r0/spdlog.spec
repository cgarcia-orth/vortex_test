Summary: spdlog version 1.3.1-r0
Name: spdlog
Version: 1.3.1
Release: r0
License: MIT
Group: base
Packager: FSL Community Team <meta-freescale@yoctoproject.org>
URL: https://github.com/gabime/spdlog/wiki
BuildRequires: cmake-native
BuildRequires: ninja-native
BuildRequires: virtual/arm-fslc-linux-gnueabi-compilerlibs
BuildRequires: virtual/arm-fslc-linux-gnueabi-gcc
BuildRequires: virtual/libc

%description
Very fast, header only, C++ logging library.

%package -n spdlog-dbg
Summary: spdlog version 1.3.1-r0 - Debugging files
License: MIT
Group: devel
Recommends: spdlog-dev = 1.3.1-r0

%description -n spdlog-dbg
Very fast, header only, C++ logging library.  This package contains ELF
symbols and related sources for debugging purposes.

%package -n spdlog-staticdev
Summary: spdlog version 1.3.1-r0 - Development files (Static Libraries)
License: MIT
Group: devel
Requires: spdlog-dev = 1.3.1-r0

%description -n spdlog-staticdev
Very fast, header only, C++ logging library.  This package contains static
libraries for software development.

%package -n spdlog-dev
Summary: spdlog version 1.3.1-r0 - Development files
License: MIT
Group: devel

%description -n spdlog-dev
Very fast, header only, C++ logging library.  This package contains
symbolic links, header files, and related items necessary for software
development.

%package -n spdlog-doc
Summary: spdlog version 1.3.1-r0 - Documentation files
License: MIT
Group: doc

%description -n spdlog-doc
Very fast, header only, C++ logging library.  This package contains
documentation.

%package -n spdlog-locale
Summary: spdlog version 1.3.1-r0
License: MIT
Group: base

%description -n spdlog-locale
Very fast, header only, C++ logging library.

%files -n spdlog-dbg
%defattr(-,-,-,-)

%files -n spdlog-dev
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/include"
%dir "/usr/lib"
%dir "/usr/include/spdlog"
%dir "/usr/include/spdlog/fmt"
%dir "/usr/include/spdlog/details"
%dir "/usr/include/spdlog/sinks"
"/usr/include/spdlog/formatter.h"
"/usr/include/spdlog/async.h"
"/usr/include/spdlog/version.h"
"/usr/include/spdlog/common.h"
"/usr/include/spdlog/tweakme.h"
"/usr/include/spdlog/logger.h"
"/usr/include/spdlog/async_logger.h"
"/usr/include/spdlog/spdlog.h"
%dir "/usr/include/spdlog/fmt/bundled"
"/usr/include/spdlog/fmt/fmt.h"
"/usr/include/spdlog/fmt/bin_to_hex.h"
"/usr/include/spdlog/fmt/ostr.h"
"/usr/include/spdlog/fmt/bundled/locale.h"
"/usr/include/spdlog/fmt/bundled/format-inl.h"
"/usr/include/spdlog/fmt/bundled/ranges.h"
"/usr/include/spdlog/fmt/bundled/chrono.h"
"/usr/include/spdlog/fmt/bundled/posix.h"
"/usr/include/spdlog/fmt/bundled/printf.h"
"/usr/include/spdlog/fmt/bundled/time.h"
"/usr/include/spdlog/fmt/bundled/format.h"
"/usr/include/spdlog/fmt/bundled/core.h"
"/usr/include/spdlog/fmt/bundled/ostream.h"
"/usr/include/spdlog/fmt/bundled/color.h"
"/usr/include/spdlog/fmt/bundled/LICENSE.rst"
"/usr/include/spdlog/details/periodic_worker.h"
"/usr/include/spdlog/details/fmt_helper.h"
"/usr/include/spdlog/details/console_globals.h"
"/usr/include/spdlog/details/thread_pool.h"
"/usr/include/spdlog/details/pattern_formatter.h"
"/usr/include/spdlog/details/async_logger_impl.h"
"/usr/include/spdlog/details/registry.h"
"/usr/include/spdlog/details/file_helper.h"
"/usr/include/spdlog/details/logger_impl.h"
"/usr/include/spdlog/details/os.h"
"/usr/include/spdlog/details/log_msg.h"
"/usr/include/spdlog/details/mpmc_blocking_q.h"
"/usr/include/spdlog/details/circular_q.h"
"/usr/include/spdlog/details/null_mutex.h"
"/usr/include/spdlog/sinks/daily_file_sink.h"
"/usr/include/spdlog/sinks/wincolor_sink.h"
"/usr/include/spdlog/sinks/ostream_sink.h"
"/usr/include/spdlog/sinks/basic_file_sink.h"
"/usr/include/spdlog/sinks/ansicolor_sink.h"
"/usr/include/spdlog/sinks/base_sink.h"
"/usr/include/spdlog/sinks/stdout_sinks.h"
"/usr/include/spdlog/sinks/android_sink.h"
"/usr/include/spdlog/sinks/null_sink.h"
"/usr/include/spdlog/sinks/rotating_file_sink.h"
"/usr/include/spdlog/sinks/dist_sink.h"
"/usr/include/spdlog/sinks/msvc_sink.h"
"/usr/include/spdlog/sinks/syslog_sink.h"
"/usr/include/spdlog/sinks/stdout_color_sinks.h"
"/usr/include/spdlog/sinks/sink.h"
%dir "/usr/lib/cmake"
%dir "/usr/lib/pkgconfig"
%dir "/usr/lib/cmake/spdlog"
"/usr/lib/cmake/spdlog/spdlogTargets.cmake"
"/usr/lib/cmake/spdlog/spdlogConfigVersion.cmake"
"/usr/lib/cmake/spdlog/spdlogConfig.cmake"
"/usr/lib/pkgconfig/spdlog.pc"

