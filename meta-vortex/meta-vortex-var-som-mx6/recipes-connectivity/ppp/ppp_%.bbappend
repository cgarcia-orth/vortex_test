FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://08setupdns-vortex \
    file://ip-up-vortex \
    file://pppd.path \
"

inherit systemd

SYSTEMD_SERVICE_${PN} = "pppd.path"
SYSTEMD_AUTO_ENABLE_${PN} = "disable"

do_install_append() {
    rm -f ${D}${sysconfdir}/ppp/ip-up.d/08setupdns
    install -m 0755 ${WORKDIR}/08setupdns-vortex ${D}${sysconfdir}/ppp/ip-up.d/08setupdns

    rm -f ${D}${sysconfdir}/ppp/ip-up.d/ip-up
    install -m 0755 ${WORKDIR}/ip-up-vortex ${D}${sysconfdir}/ppp/ip-up
    
    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/pppd.path" "${D}${systemd_unitdir}/system"
}

