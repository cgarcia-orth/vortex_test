SUMMARY = "wrapper around sd_notify for using systemd as a node process manager"
HOMEPAGE = "https://github.com/roryrjb/sd-notify#readme"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=541bbef96127f5292e6e8c31b61d698e"

SRC_URI = "npm://registry.npmjs.org/;name=sd-notify;version=${PV}"

NPM_SHRINKWRAP := "${THISDIR}/${PN}/npm-shrinkwrap.json"
NPM_LOCKDOWN := "${THISDIR}/${PN}/lockdown.json"

inherit npm

DEPENDS += "systemd"

# Must be set after inherit npm since that itself sets S
S = "${WORKDIR}/npmpkg"
LICENSE_${PN}-bindings = "MIT"
LICENSE_${PN} = "MIT"



