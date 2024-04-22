DESCRIPTION = "udev rules and script for usb tethering"
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = " \
    file://iphone-pairing.sh \
    file://iphone-pairing.rules \
"

RDEPENDS_${PN} += "bash libxml2-utils"

FILES_${PN} += " \
    ${bindir}/iphone-pairing.sh \
"

do_install() {
    # Install script
    install -d ${D}${bindir}
    install -m 0755 "${WORKDIR}/iphone-pairing.sh" "${D}${bindir}"

    # Install udev rules
    install -d ${D}/etc/udev/rules.d
    install -m 0666 "${WORKDIR}/iphone-pairing.rules" "${D}/etc/udev/rules.d/iphone-pairing.rules"
}

