DESCRIPTION = "Install rootfs files for systemd configuration."

LICENSE = "CLOSED"

SRC_URI = " \
    file://system.conf \
"

FILES_${PN} = " \
    ${sysconfdir}/systemd/system.conf \
"

do_install () {
    install -d ${D}${sysconfdir}/systemd/
    install -m 755 ${WORKDIR}/system.conf ${D}${sysconfdir}/systemd/
}
