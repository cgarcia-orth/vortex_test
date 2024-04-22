inherit core-image distro_features_check

LICENSE = "CLOSED"

# The touch screen conifguration for vortex only works with X11 for now.
DISTRO_FEATURES_remove = "wayland"
DISTRO_FEATURES_append = " \
    x11  \
    systemd \
"
VIRTUAL-RUNTIME_init_manager = "systemd"

# Tools debug and sdk could be in a dev only image.
IMAGE_FEATURES += " \
    x11-base \
    splash \
    package-management \
    tools-debug \
    tools-sdk \
    hwcodecs \
"

CORE_IMAGE_EXTRA_INSTALL += " \
    packagegroup-fsl-tools-gpu \
    packagegroup-fsl-tools-gpu-external \
    openssh \
    packagegroup-core-full-cmdline \
    openssl \
    htop \
"

# Note that rsync could be in a dev only image
IMAGE_INSTALL += "\
    vortex-touch-screen \
    u-boot-splash \
    vortex-systemd-conf \
    x11-config \
    libxkbcommon \
    nano \
    rsync \
    vortex-init \
    maestro-led-ui \
"

# Remove matchbox-terminal
IMAGE_INSTALL_remove = "matchbox-terminal"

# Set root password
inherit extrausers
EXTRA_USERS_PARAMS = "usermod -P vortex20100 root;"

# Due to the Qt tools (translations), the resulting image will not fit the default NAND size.
# Removing default ubi creation for this image.
IMAGE_FSTYPES_remove = "multiubi"

