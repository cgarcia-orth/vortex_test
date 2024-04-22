require vortex-common-image.inc

SUMMARY = "Vortex image that runs from an SD card to load a new image into a maestro box's NAND and emmc."

IMAGE_INSTALL += "\
    mtd-utils \
    maestro-load \
"

