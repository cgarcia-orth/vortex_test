FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += "file://influxdb.conf"
do_configure_append () {
    mv ${S}/../influxdb.conf ${S}/etc/influxdb/influxdb.conf
}
