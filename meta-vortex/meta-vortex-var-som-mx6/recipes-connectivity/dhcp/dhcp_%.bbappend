FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://dhcpd-vortex.conf \
    file://dhcp-server-vortex \
    file://dhcpd.service \
"

inherit systemd

SYSTEMD_SERVICE_${PN} = "dhcpd.service"
SYSTEMD_AUTO_ENABLE_${PN} = "disable"

do_install_append() {
    # select eth0 as interface for DHCP server
    rm -f ${D}${sysconfdir}/default/dhcp-server
    install -m 0644 ${WORKDIR}/dhcp-server-vortex ${D}${sysconfdir}/default/dhcp-server
    
    #DHCPD configuration
    rm -f ${D}${sysconfdir}/dhcp/dhcpd.conf
    install -m 0644 ${WORKDIR}/dhcpd-vortex.conf ${D}${sysconfdir}/dhcp/dhcpd.conf
  
    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/dhcpd.service" "${D}${systemd_unitdir}/system"
}