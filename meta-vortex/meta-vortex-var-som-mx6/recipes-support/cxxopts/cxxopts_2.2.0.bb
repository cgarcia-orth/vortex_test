DESCRIPTION = "Lightweight C++ command line option parser."
HOMEPAGE = "https://github.com/jarro2783/cxxopts"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "e6858d3429e0ba5fe6f42ce2018069ce992f3d26"
SRC_URI = "git://github.com/jarro2783/cxxopts.git;"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

inherit cmake

# Header-only library
RDEPENDS_${PN}-dev = ""
RRECOMMENDS_${PN}-dbg = "${PN}-dev (= ${EXTENDPKGV})"

