Summary: libsole version 1.0.1-r0
Name: libsole
Version: 1.0.1
Release: r0
License: Zlib
Group: base
Packager: FSL Community Team <meta-freescale@yoctoproject.org>
URL: https://github.com/r-lyeh-archived/sole
BuildRequires: virtual/arm-fslc-linux-gnueabi-compilerlibs
BuildRequires: virtual/arm-fslc-linux-gnueabi-gcc
BuildRequires: virtual/libc

%description
Sole is a lightweight C++11 library to generate universally unique
identificators (UUID), both v1 and v4.

%package -n libsole-dbg
Summary: libsole version 1.0.1-r0 - Debugging files
License: Zlib
Group: devel
Recommends: libsole-dev = 1.0.1-r0

%description -n libsole-dbg
Sole is a lightweight C++11 library to generate universally unique
identificators (UUID), both v1 and v4.  This package contains ELF symbols
and related sources for debugging purposes.

%package -n libsole-staticdev
Summary: libsole version 1.0.1-r0 - Development files (Static Libraries)
License: Zlib
Group: devel
Requires: libsole-dev = 1.0.1-r0

%description -n libsole-staticdev
Sole is a lightweight C++11 library to generate universally unique
identificators (UUID), both v1 and v4.  This package contains static
libraries for software development.

%package -n libsole-dev
Summary: libsole version 1.0.1-r0 - Development files
License: Zlib
Group: devel

%description -n libsole-dev
Sole is a lightweight C++11 library to generate universally unique
identificators (UUID), both v1 and v4.  This package contains symbolic
links, header files, and related items necessary for software development.

%package -n libsole-doc
Summary: libsole version 1.0.1-r0 - Documentation files
License: Zlib
Group: doc

%description -n libsole-doc
Sole is a lightweight C++11 library to generate universally unique
identificators (UUID), both v1 and v4.  This package contains
documentation.

%package -n libsole-locale
Summary: libsole version 1.0.1-r0
License: Zlib
Group: base

%description -n libsole-locale
Sole is a lightweight C++11 library to generate universally unique
identificators (UUID), both v1 and v4.

%files -n libsole-dbg
%defattr(-,-,-,-)

%files -n libsole-dev
%defattr(-,-,-,-)
%dir "/usr"
%dir "/usr/include"
"/usr/include/sole.hpp"

