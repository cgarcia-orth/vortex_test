DESCRIPTION = "Light-weight, simple and fast XML parser for C++ with XPath support"
HOMEPAGE = "https://github.com/zeux/pugixml"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "3c59df555b93a7650ae0056da34bacde93f4ef8f"
SRC_URI = "git://github.com/zeux/pugixml.git"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

inherit cmake

EXTRA_OECMAKE += "-DBUILD_SHARED_LIBS=ON"

RDEPENDS_${PN}-dev = ""
RRECOMMENDS_${PN}-dbg = "${PN}-dev (= ${EXTENDPKGV})"

