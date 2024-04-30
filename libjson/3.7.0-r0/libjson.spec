Summary: libjson version 3.7.0-r0
Name: libjson
Version: 3.7.0
Release: r0
License: MIT
Group: base
Packager: FSL Community Team <meta-freescale@yoctoproject.org>
URL: https://github.com/nlohmann/json
BuildRequires: cmake-native
BuildRequires: ninja-native
BuildRequires: virtual/arm-fslc-linux-gnueabi-compilerlibs
BuildRequires: virtual/arm-fslc-linux-gnueabi-gcc
BuildRequires: virtual/libc

%description
Open Source library to serialize/unserialize JSON messages.

%package -n libjson-dbg
Summary: libjson version 3.7.0-r0 - Debugging files
License: MIT
Group: devel
Recommends: libjson-dev = 3.7.0-r0

%description -n libjson-dbg
Open Source library to serialize/unserialize JSON messages.  This package
contains ELF symbols and related sources for debugging purposes.

%package -n libjson-staticdev
Summary: libjson version 3.7.0-r0 - Development files (Static Libraries)
License: MIT
Group: devel
Requires: libjson-dev = 3.7.0-r0

%description -n libjson-staticdev
Open Source library to serialize/unserialize JSON messages.  This package
contains static libraries for software development.

%package -n libjson-dev
Summary: libjson version 3.7.0-r0 - Development files
License: MIT
Group: devel

%description -n libjson-dev
Open Source library to serialize/unserialize JSON messages.  This package
contains symbolic links, header files, and related items necessary for
software development.

%package -n libjson-doc
Summary: libjson version 3.7.0-r0 - Documentation files
License: MIT
Group: doc

%description -n libjson-doc
Open Source library to serialize/unserialize JSON messages.  This package
contains documentation.

%package -n libjson-locale
Summary: libjson version 3.7.0-r0
License: MIT
Group: base

%description -n libjson-locale
Open Source library to serialize/unserialize JSON messages.

%files -n libjson-dbg
%defattr(-,-,-,-)

%files -n libjson-dev
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/include"
%dir "/usr/lib"
%dir "/usr/include/nlohmann"
"/usr/include/nlohmann/json.hpp"
%dir "/usr/lib/cmake"
%dir "/usr/lib/cmake/nlohmann_json"
"/usr/lib/cmake/nlohmann_json/nlohmann_jsonTargets.cmake"
"/usr/lib/cmake/nlohmann_json/nlohmann_jsonConfigVersion.cmake"
"/usr/lib/cmake/nlohmann_json/nlohmann_jsonConfig.cmake"

