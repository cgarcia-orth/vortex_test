DESCRIPTION = "Install rootfs files to make touch screen work properly."

LICENSE = "CLOSED"

SRC_URI = " \
    file://.xsessionrc \
    file://.Xsession \
"

FILES_${PN} = " \
    /home/root/.xsessionrc \
    /home/root/.Xsession \
"

do_install () {
    install -d ${D}/home/root
    install -m 755 ${WORKDIR}/.Xsession ${D}/home/root/
    install -m 644 ${WORKDIR}/.xsessionrc ${D}/home/root/
}
