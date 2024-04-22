FILESEXTRAPATHS_prepend := "${THISDIR}/files:"


SRC_URI += " \
    file://blacklistVortex.conf \
"

do_install_append() {
    # Remove the autonet rules to prevent conflicts between vortex scripts and this.
    rm -f ${D}${sysconfdir}/udev/rules.d/autonet.rules
    
    # prevent rndis loading
    rm -f ${D}${sysconfdir}/modprobe.d/blacklist.conf
    install -m 0644 ${WORKDIR}/blacklistVortex.conf ${D}${sysconfdir}/modprobe.d/blacklist.conf
}
