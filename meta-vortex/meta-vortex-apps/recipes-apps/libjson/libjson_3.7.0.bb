DESCRIPTION = "Open Source library to serialize/unserialize JSON messages."
HOMEPAGE = "https://github.com/nlohmann/json"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "53c3eefa2cf790a7130fed3e13a3be35c2f2ace2"
SRC_URI = "git://github.com/nlohmann/json.git;nobranch=1;"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

inherit cmake

# Header-only library
RDEPENDS_${PN}-dev = ""
RRECOMMENDS_${PN}-dbg = "${PN}-dev (= ${EXTENDPKGV})"

