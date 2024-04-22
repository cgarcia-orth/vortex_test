DESCRIPTION = "Quectel connection files"
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = " \
    file://auto-connect-pppd \
    file://do-not-auto-connect-pppd \
    file://quectel-chat-connect-template \
    file://quectel-ppp-template \
    file://pppd.service \
"

RDEPENDS_${PN} += "ppp"

FILES_${PN} += " \
    /opt/vortex/quectel/auto-connect-pppd \
    /opt/vortex/quectel/do-not-auto-connect-pppd \
    /opt/vortex/quectel/quectel-chat-connect-template \
    /opt/vortex/quectel/quectel-ppp-template \
    ${systemd_unitdir}/system/pppd.service \
"
do_install() {
    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/pppd.service" "${D}${systemd_unitdir}/system"

    # Install script
    install -d ${D}/opt/vortex/quectel
    install -t ${D}/opt/vortex/quectel/ -m 0777 "${WORKDIR}/auto-connect-pppd"
    install -t ${D}/opt/vortex/quectel/ -m 0777 "${WORKDIR}/do-not-auto-connect-pppd"
    install -t ${D}/opt/vortex/quectel/ -m 0666 "${WORKDIR}/quectel-chat-connect-template"
    install -t ${D}/opt/vortex/quectel/ -m 0666 "${WORKDIR}/quectel-ppp-template"
}

