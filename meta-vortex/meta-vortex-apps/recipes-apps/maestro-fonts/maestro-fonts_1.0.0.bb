require recipes-graphics/ttf-fonts/ttf.inc
DESCRIPTION = "Maestro Fonts"
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = "file://fonts"

FILES_${PN} = "${sysconfdir} ${datadir}"

S = "${WORKDIR}/fonts"

do_install_append () {
    install -d ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-Black.ttf ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-BlackItalic.ttf ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-Bold.ttf ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-BoldItalic.ttf ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-Book.ttf ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-BookItalic.ttf ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-Medium.ttf ${D}${sysconfdir}/fonts/conf.d/
    install -m 0644 ${WORKDIR}/fonts/Gotham-MediumItalic.ttf ${D}${sysconfdir}/fonts/conf.d/
}

