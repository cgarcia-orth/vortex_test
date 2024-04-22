DESCRIPTION = "MockComMgr"
LICENSE = "CLOSED"
PR = "r0"

FILESPATH = "${THISDIR}/files:${VORTEX_ROOT}/vortex/"

SRC_URI = " \
    file://MockComMgr \
    file://mock-com-mgr.service \
"

RDEPENDS_${PN} += "vortex-core"
DEPENDS += "vortex-core cxxopts"

inherit cmake systemd

S = "${WORKDIR}"

export CMAKE_ROOT_DIR="${STAGING_DIR_HOST}"

FILES_${PN} += " \
    ${includedir}/*.h \
    ${includedir}/ \
    ${systemd_unitdir}/system/mock-com-mgr.service \
"

FILES_${PN}-dev += " \
    ${includedir}/*.h \
    ${includedir}/ \
"

SYSTEMD_SERVICE_${PN} = "mock-com-mgr.service"

OECMAKE_SOURCEPATH = "${S}/MockComMgr/src"

do_install() {
    # Install application
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/build/MockComMgr ${D}${bindir}/MockComMgr

    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/mock-com-mgr.service" "${D}${systemd_unitdir}/system"
}

