DESCRIPTION = "MaestroLoad"
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = " \
    file://echos.sh \
    file://install_yocto.sh \
    file://install_yocto_emmc.sh \
    file://maestro-load.service \
"

inherit systemd

RDEPENDS_${PN} += "bash"

FILES_${PN} += " \
    ${bindir}/echos.sh \
    ${bindir}/install_yocto.sh \
    ${bindir}/install_yocto_emmc.sh \
    ${systemd_unitdir}/system/maestro-load.service \
"

SYSTEMD_SERVICE_${PN} = "maestro-load.service"

do_install() {
    # Install scripts
    install -d ${D}${bindir}
    install -m 0755 "${WORKDIR}/echos.sh" "${D}${bindir}"
    install -m 0755 "${WORKDIR}/install_yocto.sh" "${D}${bindir}"
    install -m 0755 "${WORKDIR}/install_yocto_emmc.sh" "${D}${bindir}"

    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/maestro-load.service" "${D}${systemd_unitdir}/system"
}

