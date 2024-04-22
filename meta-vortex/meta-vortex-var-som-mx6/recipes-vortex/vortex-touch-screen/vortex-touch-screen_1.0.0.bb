DESCRIPTION = "Install rootfs files to make touch screen work properly."

LICENSE = "CLOSED"

SRC_URI = " \
    file://xorg.conf \
"

FILES_${PN} = " \
    ${sysconfdir}/X11/xorg.conf \
"

do_install () {
    install -d ${D}${sysconfdir}/X11/
    install -m 755 ${WORKDIR}/xorg.conf ${D}${sysconfdir}/X11/
}
