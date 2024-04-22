DESCRIPTION = "MaestroPro"
LICENSE = "CLOSED"
PR = "r0"

FILESPATH = "${THISDIR}/files:${VORTEX_ROOT}/vortex/"

SRC_URI = " \
    file://MaestroPro \
    file://maestro-pro.service \
    file://MaestroProPreStartScripts.sh \
"

RDEPENDS_${PN} += "vortex-core libartnet libmodbus pugixml influxdb bash libxml2-utils nmap"
DEPENDS += "vortex-core libartnet libmodbus cxxopts pugixml influxdb-cpp date"

INSANE_SKIP_${PN} += "already-stripped"

inherit cmake systemd

S = "${WORKDIR}"

export CMAKE_ROOT_DIR="${STAGING_DIR_HOST}"
EXTRA_OECMAKE += "-DMAESTRO_PRO_VERSION=${PV}"

FILES_${PN} += " \
    ${includedir}/*.h \
    ${includedir}/ \
    ${systemd_unitdir}/system/maestro-pro.service \
    /opt/vortex/basic_ts_files \
"

FILES_${PN}-dev += " \
    ${includedir}/*.h \
    ${includedir}/ \
"

SYSTEMD_SERVICE_${PN} = "maestro-pro.service"

OECMAKE_SOURCEPATH = "${S}/MaestroPro/src"

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
    install -m 0755 ${WORKDIR}/build/MaestroPro ${D}${bindir}/MaestroPro
    install -m 0755 ${WORKDIR}/MaestroProPreStartScripts.sh ${D}${bindir}/MaestroProPreStartScripts.sh

    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/maestro-pro.service" "${D}${systemd_unitdir}/system"

    # Install CAN CLI
    install -m 0755 ${WORKDIR}/build/MaestroProCan ${D}${bindir}/MaestroProCan

    # Install .Ts
    [ -d ${D}/opt/vortex/basic_ts_files ] || install -d ${D}/opt/vortex/basic_ts_files
    install -m 0755 "${WORKDIR}/MaestroPro/basic_ts_files/MaestroPRO_Fr.ts" "${D}/opt/vortex/basic_ts_files"
    install -m 0755 "${WORKDIR}/MaestroPro/basic_ts_files/MaestroPRO_Sp.ts" "${D}/opt/vortex/basic_ts_files"
}

