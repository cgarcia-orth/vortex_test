DESCRIPTION = "An Open Source implementation of the ArtNet protocol."
HOMEPAGE = "https://github.com/OpenLightingProject/libartnet"
LICENSE = "LGPL-2.1"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/LGPL-2.1;md5=1a6d268fd218675ffea8be556788b780"

SRCREV = "713efee8b0d9dfd19d5267fca0685810e093c2fa"
SRC_URI = " \
    git://github.com/OpenLightingProject/libartnet.git;protocol=git \
    file://libarnet.diff \
"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

inherit autotools

autotools_preconfigure_append() {
    bbnote "Will execute autoreconf -i ${S}"
    autoreconf -i "${S}"
}


