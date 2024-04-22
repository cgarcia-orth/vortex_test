DESCRIPTION = "A Modbus library for Linux, Mac OS X, FreeBSD, QNX and Windows."
HOMEPAGE = "http://libmodbus.org"
LICENSE = "LGPL-2.1"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/LGPL-2.1;md5=1a6d268fd218675ffea8be556788b780"

SRCREV = "f9fe3b0a5343f7fbb3f5c74196bd0fce88df39d5"
SRC_URI = " \
    git://github.com/stephane/libmodbus.git;protocol=git \
"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

inherit autotools

autotools_preconfigure_append() {
    bbnote "Will execute autoreconf -i ${S}"
    autoreconf -i "${S}"
}

