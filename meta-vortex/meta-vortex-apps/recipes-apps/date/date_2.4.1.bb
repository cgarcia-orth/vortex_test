DESCRIPTION = "A date and time library based on the C++11/14/17 <chrono> header"
HOMEPAGE = "https://github.com/HowardHinnant/date"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "e7e1482087f58913b80a20b04d5c58d9d6d90155"
SRC_URI = "git://github.com/HowardHinnant/date.git;nobranch=1;"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

inherit cmake

RDEPENDS_${PN}-dev = ""
RRECOMMENDS_${PN}-dbg = "${PN}-dev (= ${EXTENDPKGV})"

