DESCRIPTION = "Maestro LED UI"
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = " \
    file://maestro-led-ui.sh \
"

RDEPENDS_${PN} += "bash"

FILES_${PN} += " \
    ${bindir}/maestro-led-ui.sh \
"

do_install() {
    # Install script
    install -d ${D}${bindir}
    install -m 0755 "${WORKDIR}/maestro-led-ui.sh" "${D}${bindir}"
}

