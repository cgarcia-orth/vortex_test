# We do this because the vortex-touch-screen recipe installs a different file 
# at that location. There probably is another way to do this. However, this 
# works and I am out of time - Olivier.
do_install_append () {
    rm -f ${D}${sysconfdir}/X11/xorg.conf
}
