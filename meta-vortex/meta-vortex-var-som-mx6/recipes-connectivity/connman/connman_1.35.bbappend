FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

# Looks like ofono is not used to manage ppp connections.
PACKAGECONFIG_append = " 3g"

SRC_URI += "file://main.conf"

FILES_${PN} += " \
    /etc/conman/main.conf \
    "

do_install_append() {
    # Install config
    install -d "${D}/etc/connman"
    install -t "${D}/etc/connman" "${WORKDIR}/main.conf"

    # make sure connman share same resolv.conf as pppd
    install -d ${D}/etc
    ln -fs ../run/resolv.conf ${D}/etc/resolv.conf

    # remove connman capability to set system time
    sed -i 's/CAP_SYS_TIME //' ${D}${systemd_unitdir}/system/connman.service
}

