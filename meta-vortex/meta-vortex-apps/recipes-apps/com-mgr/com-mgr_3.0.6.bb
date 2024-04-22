DESCRIPTION = "ComMgr"
LICENSE = "CLOSED"
PR = "r0"

FILESPATH = "${THISDIR}/files"

SRC_URI = " \
    file://communication-manager \
    file://com-mgr.service \
"
DEPENDS += "rsync-native"
RDEPENDS_${PN} += "nodejs sd-notify bash"

inherit systemd

S = "${WORKDIR}"

FILES_${PN} += " \
    ${systemd_unitdir}/system/com-mgr.service \
    ${bindir}/comm_manager/ \
    ${bindir}/comm_manager/* \
"

SYSTEMD_SERVICE_${PN} = "com-mgr.service"

INSANE_SKIP_${PN} += "host-user-contaminated"

# Prevent services from starting after install
systemd_postinst() {
OPTS=""

if [ -n "$D" ]; then
    OPTS="--root=$D"
fi

if type systemctl >/dev/null 2>/dev/null; then
	if [ -z "$D" ]; then
		systemctl daemon-reload
	fi

	systemctl $OPTS ${SYSTEMD_AUTO_ENABLE} ${SYSTEMD_SERVICE_ESCAPED}
fi
}

do_install() {
    # Install application
    install -d ${D}${bindir}/comm_manager
    rsync -a ${S}/communication-manager/ ${D}${bindir}/comm_manager/

    # Install service file
    install -d "${D}${systemd_unitdir}/system"
    install -m 0644 "${WORKDIR}/com-mgr.service" "${D}${systemd_unitdir}/system"
}

