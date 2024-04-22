DESCRIPTION = "Sole is a lightweight C++11 library to generate universally unique identificators (UUID), both v1 and v4."
HOMEPAGE = "https://github.com/r-lyeh-archived/sole"
LICENSE = "Zlib"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Zlib;md5=87f239f408daca8a157858e192597633"

SRCREV = "78e2e209af306c81a85e485f8ce0f3768ab1625f"
SRC_URI = "git://github.com/r-lyeh-archived/sole.git;nobranch=1;"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

FILES_${PN} += " \
    ${includedir}/*.hpp \
    ${includedir} \
"

FILES_${PN}-dev += " \
    ${includedir}/*.hpp \
    ${includedir}/ \
"

do_install() {
    # Install script
    install -d ${D}${includedir}
    install -m 0755 "${S}/sole.hpp" "${D}${includedir}"
}

# Header-only library
RDEPENDS_${PN}-dev = ""
RRECOMMENDS_${PN}-dbg = "${PN}-dev (= ${EXTENDPKGV})"

