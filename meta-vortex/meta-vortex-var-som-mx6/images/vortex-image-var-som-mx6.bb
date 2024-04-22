require vortex-common-image.inc
inherit populate_sdk_qt5

SUMMARY = "Vortex image for maestro"

IMAGE_INSTALL += "\
    maestro-gui \
    maestro-pro \
    com-mgr \
    connman-client \
    quectel \
    usbmuxd \
    udev-extraconf \
    check-update \
    vortex-update \
    vortex-usb-wan \
    libimobiledevice \
    dhcp-server \
    nmap \
"

# 4GB in kB
IMAGE_ROOTFS_SIZE = "4194304"
# Minimum is 1.3
# IMAGE_OVERHEAD_FACTOR = "1.3"
IMAGE_ROOTFS_EXTRA_SPACE = "0"

# SDK setup for libs
TOOLCHAIN_HOST_TASK += "nativesdk-gcc nativesdk-vortex-core nativesdk-vortex-core-dev nativesdk-libartnet nativesdk-libartnet-dev nativesdk-libmodbus nativesdk-libmodbus-dev nativesdk-pugixml nativesdk-pugixml-dev nativesdk-influxdb-cpp-dev nativesdk-date-dev"

