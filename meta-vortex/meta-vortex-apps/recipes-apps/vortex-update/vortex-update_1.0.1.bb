DESCRIPTION = "Update files and applications."
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = " \
    file://vortex-update.sh \
"

RDEPENDS_${PN} += "bash libxml2-utils"

FILES_${PN} += " \
    ${bindir}/check-update.sh \
"

do_install() {
    # Install script
    install -d ${D}${bindir}
    install -m 0755 "${WORKDIR}/vortex-update.sh" "${D}${bindir}"
}

