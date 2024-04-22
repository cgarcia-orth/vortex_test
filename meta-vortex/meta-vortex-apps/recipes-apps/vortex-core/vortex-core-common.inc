DESCRIPTION = "VortexCore"
LICENSE = "CLOSED"
PR = "r0"

FILESPATH = "${VORTEX_ROOT}/vortex/"

SRC_URI = " \
    file://VortexCore \
"

inherit cmake

S = "${WORKDIR}"

export CMAKE_ROOT_DIR="${STAGING_DIR_HOST}"

FILES_${PN} += " \
    ${includedir}/*.h \
    ${includedir}/ \
"

FILES_${PN}-dev += " \
    ${includedir}/*.h \
    ${includedir}/ \
"

DEPENDS += "rsync-native spdlog libjson gtest libsole"

OECMAKE_SOURCEPATH = "${S}/VortexCore/src"

do_install() {
    install -d ${D}${libdir}
    install -m 0755 ${S}/build/libVortexCore.so ${D}${libdir}/libVortexCore.so.0
    ln -s libVortexCore.so.0 ${D}${libdir}/libVortexCore.so
    install -m 0755 ${S}/build/libVortexCore.a ${D}${libdir}

    install -d ${D}${includedir}/VortexCore/
    cp -r ${S}/VortexCore/includes ${D}${includedir}/VortexCore/
    install -d ${D}${includedir}/VortexCore/src
    rsync -a --include='*.h' --include='*/' --exclude='*' ${S}/VortexCore/src/ ${D}${includedir}/VortexCore/src
}


