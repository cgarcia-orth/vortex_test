FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += " \
    file://vortex-logrotate.conf \
    file://syslog \
    file://logrotate \
"

FILES_${PN} += " \
    ${sysconfdir}/logrotate.d/syslog \
    ${sysconfdir}/cron.d/logrotate \
"

do_install_append () {
    # Replace the config file
    rm -f ${D}${sysconfdir}/logrotate.conf
    install -p -m 644 ${S}/../vortex-logrotate.conf ${D}${sysconfdir}/logrotate.conf

    # Install files to run the rotation
    install -d ${D}${sysconfdir}/logrotate.d
    install -m 644 ${S}/../syslog ${D}${sysconfdir}/logrotate.d/syslog
    install -d ${D}${sysconfdir}/cron.d
    install -m 644 ${S}/../logrotate ${D}${sysconfdir}/cron.d/logrotate
}
