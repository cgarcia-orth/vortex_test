DESCRIPTION = "MaestroGUI"
LICENSE = "CLOSED"
PR = "r0"

FILESPATH = "${THISDIR}/files:${VORTEX_ROOT}/vortex/"

SRC_URI = " \
    file://MaestroGUI \
    file://maestro-gui.service \
"


RDEPENDS_${PN} += "vortex-core qtbase qtquickcontrols qtquickcontrols2 qtdeclarative libxkbcommon qtvirtualkeyboard maestro-fonts qttools-tools"
DEPENDS += "vortex-core qtbase qtquickcontrols qtquickcontrols2 qtdeclarative libxkbcommon qtvirtualkeyboard"

inherit qmake5 systemd

S = "${WORKDIR}/MaestroGUI/MaestroGUI"

export MAESTRO_GUI_VERSION = "${PV}"

FILES_${PN} += " \
    ${systemd_unitdir}/system/maestro-gui.service \
    ${bindir}/MaestroGUI \
    /opt/vortex/basic_ts_files \
    /opt/vortex/images \
"

SYSTEMD_SERVICE_${PN} = "maestro-gui.service"

# Prevent services from starting after install
systemd_postinst() {
OPTS=""

if [ -n "$D" ]; then
    OPTS="--root=$D"
fi

if type systemctl >/dev/null 2>/dev/null; then
	if [ -z "$D" ]; then
		systemctl daemon-reload
	fi

	systemctl $OPTS ${SYSTEMD_AUTO_ENABLE} ${SYSTEMD_SERVICE_ESCAPED}
fi
}

do_install() {
    # Install application
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/build/MaestroGUI ${D}${bindir}/MaestroGUI

    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/maestro-gui.service" "${D}${systemd_unitdir}/system"

    # Install ts files
    [ -d ${D}/opt/vortex/basic_ts_files ] || install -d ${D}/opt/vortex/basic_ts_files
    install -m 0755 "${WORKDIR}/MaestroGUI/basic_ts_files/MaestroGUI_Fr.ts" "${D}/opt/vortex/basic_ts_files"
    install -m 0755 "${WORKDIR}/MaestroGUI/basic_ts_files/MaestroGUI_Sp.ts" "${D}/opt/vortex/basic_ts_files"

    # Install help images
    [ -d ${D}/opt/vortex/images ] || install -d ${D}/opt/vortex/images
    cp -r ${WORKDIR}/MaestroGUI/images/helpImages ${D}/opt/vortex/images
}

