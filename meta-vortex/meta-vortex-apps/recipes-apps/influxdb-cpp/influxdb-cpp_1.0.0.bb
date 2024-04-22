DESCRIPTION = "A header-only C++ query & write client for InfluxDB."
HOMEPAGE = "https://github.com/orca-zhang/influxdb-cpp"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "5ee1f8869f6a4b7d647a884f82c1551afa16358d"
SRC_URI = "git://github.com/orca-zhang/influxdb-cpp.git;nobranch=1;"

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native nativesdk"

FILES_${PN}-dev += " \
    ${includedir}/influxdb.hpp \
"

do_install() {
    # Install script
    install -d ${D}${includedir}
    install -m 0755 "${S}/influxdb.hpp" "${D}${includedir}"
}

# Header-only library
RDEPENDS_${PN}-dev = ""
RRECOMMENDS_${PN}-dbg = "${PN}-dev (= ${EXTENDPKGV})"

