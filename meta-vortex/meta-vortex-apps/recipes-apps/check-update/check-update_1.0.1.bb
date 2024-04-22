DESCRIPTION = "Check applications helth after update."
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = " \
    file://check-update.sh \
    file://check-update.service \
"

inherit systemd

RDEPENDS_${PN} += "bash"

FILES_${PN} += " \
    ${bindir}/check-update.sh \
    ${systemd_unitdir}/system/check-update.service \
"

SYSTEMD_SERVICE_${PN} = "check-update.service"

do_install() {
    # Install script
    install -d ${D}${bindir}
    install -m 0755 "${WORKDIR}/check-update.sh" "${D}${bindir}"

    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/check-update.service" "${D}${systemd_unitdir}/system"
}

