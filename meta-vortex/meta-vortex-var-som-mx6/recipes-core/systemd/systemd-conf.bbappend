# We do this because the vortex-systemd-conf recipe installs a different file 
# at that location. There probably is another way to do this. However, this 
# works and I am out of time - Olivier.
do_install_append () {
    # Remove file so we can isntall our own
    rm -f ${D}${sysconfdir}/systemd/system.conf

    # Modify journald to limit disk size
    sed -i -e 's/.*SystemMaxUse.*/SystemMaxUse=64M/' ${D}${sysconfdir}/systemd/journald.conf
}
