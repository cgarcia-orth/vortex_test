DESCRIPTION = "Vortex Controller Initialisation"
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = " \
    file://vortex-init.sh \
    file://vortex-init.service \
"

inherit systemd

RDEPENDS_${PN} += "bash"

FILES_${PN} += " \
    ${bindir}/vortex-init.sh \
    ${systemd_unitdir}/system/vortex-init.service \
    /opt/vortex/ios \
"

SYSTEMD_SERVICE_${PN} = "vortex-init.service"

do_install() {
    # Install script
    install -d ${D}${bindir}
    install -d ${D}/opt/vortex/ios
    install -m 0755 "${WORKDIR}/vortex-init.sh" "${D}${bindir}"

    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/vortex-init.service" "${D}${systemd_unitdir}/system"
}

