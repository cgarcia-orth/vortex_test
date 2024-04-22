#!/bin/sh
#
# Vortex Maestro Build Environment Setup Script
#
sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib \
build-essential chrpath socat cpio python python3 python3-pip python3-pexpect \
xz-utils debianutils iputils-ping libsdl1.2-dev xterm

sudo apt-get install autoconf libtool libglib2.0-dev libarchive-dev python-git \
sed cvs subversion coreutils texi2html docbook-utils python-pysqlite2 \
help2man make gcc g++ desktop-file-utils libgl1-mesa-dev libglu1-mesa-dev \
mercurial automake groff curl lzop asciidoc u-boot-tools dos2unix mtd-utils pv \
libncurses5 libncurses5-dev libncursesw5-dev libelf-dev zlib1g-dev rpm

# Libs required for dev
sudo apt-get install libsystemd-dev

cd `readlink -f $(dirname $0)`
VORTEX_MAESTRO_DIR=`pwd`
MACHINE=var-som-mx6 
DISTRO=vortex-fslc-x11

if [ "$(whoami)" = "root" ]; then
    echo "ERROR: do not use the BSP as root. Exiting..."
    unset VORTEX_MAESTRO_DIR
    exit 1
fi

# Check if meta-vortex-var-som-mx6 layer exists
if [ -e ${VORTEX_MAESTRO_DIR}/meta-vortex-var-som-mx6 ]; then
    IMAGE_LAYER="${VORTEX_MAESTRO_DIR}/meta-vortex-var-som-mx6"
fi

# Git to clone
# Poky
# git clone -b thud https://git.yoctoproject.org/git/poky
# git reset --hard 50f33d3bfebcbfb1538d932fb487cfd789872026
# meta-freescale
# git clone -b thud https://git.yoctoproject.org/git/meta-freescale
# git reset --hard 46fcbab00f7e01ded4609c09be89161783426f41
# meta-openembedded
# git clone -b thud https://github.com/openembedded/meta-openembedded
# git reset --hard 4cd3a39f22a2712bfa8fc657d09fe2c7765a4005
# meta-freescale-3rdparty
# git clone -b thud https://github.com/Freescale/meta-freescale-3rdparty
# git reset --hard c4b5ac6b20e4245ce0630e9197313aaef999a331
# meta-freescale-distro
# git clone -b thud https://github.com/Freescale/meta-freescale-distro
# git reset --hard 4a244af3993ae662624c6f615464e6806cc719a2
# meta-qt5
# git clone -b thud git://github.com/meta-qt5/meta-qt5
# git reset --hard 0630018c0033c91fddda62a49f59a82ba6ec6850
# meta-swupdate
# git clone -b thud https://github.com/sbabic/meta-swupdate
# git reset --hard 66af6e7e019b07b48facfd68be3c4ab2094502a4
# meta-variscite-fslc
# git clone -b thud https://github.com/varigit/meta-variscite-fslc
# git reset --hard 73bd1c1360d923d79c557850d6818756c5a85542
# meta-influx
# git clone https://github.com/alihanyalcin/meta-influx.git
# git reset --hard 208d0ee4c53d7f2f0897547930efe13c403d2358

cd ${VORTEX_MAESTRO_DIR}

usage()
{
    echo "    Optional parameters:
    * [-j jobs]:  number of jobs for make to spawn during the compilation stage.
    * [-t tasks]: number of BitBake tasks that can be issued in parallel.
    * [-c path]:  non-default path of SSTATE_DIR (shared state Cache)
    * [-b path]:  non-default path of project folder (build_\${machine})
    * [-h]:       help
    "
}

prompt_message ()
{
    echo "Run the following commands to start a build:"
    local i=''

    if [ -e "${IMAGE_LAYER}" ]; then
        for i in `find ${IMAGE_LAYER}/ -name "vortex-image-*.bb"`;do
        i=${i##*/};i=${i%.bb};
            echo "    bitbake $i";
        done
    fi
    
    echo "To return to this build environment later please run:"
    echo "    . $PROJECT_DIR/source_me"
}

clean_up()
{
    unset VORTEX_MAESTRO_DIR PROJECT_DIR MACHINE IMAGE_LAYER \
        setup_flag setup_j setup_t setup_l setup_h \
        setup_string setup_project setup_sstate setup_error \
        APPEND_LAYER_LIST layer append_layer \
        OLD_OPTIND CPUS JOBS THREADS ARCH EULA DOWNLOADS CACHES
    unset -f prompt_message usage clean_up
}

# get command line options
OLD_OPTIND=$OPTIND
while getopts "m:j:t:b:c:h" setup_flag
do
    case $setup_flag in
        h) setup_h='true';
           ;;
        b) setup_project="$OPTARG";
           ;;
        c) setup_sstate="$OPTARG";
           ;;
        ?) setup_error='true';
           ;;
    esac
done
OPTIND=$OLD_OPTIND

# check the "-h" and other not supported options
if test $setup_error || test $setup_h; then
    usage && clean_up && exit 1
fi

# Build layer list
APPEND_LAYER_LIST=" \
    meta-vortex/meta-vortex-var-som-mx6 \
    meta-vortex/meta-vortex-apps \
    meta-freescale \
    meta-freescale-3rdparty \
    meta-freescale-distro \
    meta-qt5 \
    meta-openembedded/meta-oe \
    meta-openembedded/meta-multimedia \
    meta-openembedded/meta-gnome \
    meta-openembedded/meta-python \
    meta-openembedded/meta-networking \
    meta-openembedded/meta-filesystems \
    meta-swupdate \
    meta-variscite-fslc \
    meta-influx \
"

# check optional jobs and threads
if echo "$setup_j" | egrep -q "^[0-9]+$"; then
    JOBS=$setup_j
fi
if echo "$setup_t" | egrep -q "^[0-9]+$"; then
    THREADS=$setup_t
fi

# set project folder location and name
if test $setup_project; then
    PROJECT_DIR=`pwd`/${setup_project}
else
    PROJECT_DIR=$VORTEX_MAESTRO_DIR/../build_${MACHINE}
fi
if test $setup_string; then
    PROJECT_DIR="${PROJECT_DIR}_${setup_string}"
fi

if [ ! -d $PROJECT_DIR ]; then
    mkdir -p $PROJECT_DIR
fi

DOWNLOADS="$VORTEX_MAESTRO_DIR/../../3rd_parties"

mkdir -p $DOWNLOADS
DOWNLOADS=`readlink -f "$DOWNLOADS"`

if test $setup_sstate; then
    CACHES="$setup_sstate"
else
    CACHES="$VORTEX_MAESTRO_DIR/../sstate-cache"
fi
mkdir -p $CACHES
CACHES=`readlink -f "$CACHES"`

# check if project folder was created before
if [ -e "$PROJECT_DIR/source_me" ]; then
    echo "$PROJECT_DIR was created before. Removing source_me..."
    rm $PROJECT_DIR/source_me
fi

# source oe-init-build-env to init build env
cd $VORTEX_MAESTRO_DIR/../poky
set -- $PROJECT_DIR
. ./oe-init-build-env > /dev/null

# check result of sourcing oe-init-build-env
if [ "`pwd -P`" != "$BUILDDIR" ];then
    clean_up && exit 1
fi

# add layers
for layer in $(eval echo $APPEND_LAYER_LIST); do
    append_layer=""
    if [ -e ${VORTEX_MAESTRO_DIR}/../${layer} ]; then
        append_layer="${VORTEX_MAESTRO_DIR}/../${layer}"
    elif [ -e ${VORTEX_MAESTRO_DIR}/${layer} ]; then
        append_layer="${VORTEX_MAESTRO_DIR}/${layer}"
    fi
    if [ -n "${append_layer}" ]; then
        append_layer=`readlink -f $append_layer`
        awk '/  \"/ && !x {print "'"  ${append_layer}"' \\"; x=1} 1' \
            conf/bblayers.conf > conf/bblayers.conf~
        mv conf/bblayers.conf~ conf/bblayers.conf
    fi
done

cat >> conf/local.conf <<-EOF
MACHINE ?= "$MACHINE"
DISTRO = "$DISTRO"
DL_DIR = "$DOWNLOADS"
BB_GENERATE_MIRROR_TARBALLS = "1"
SSTATE_DIR = "$CACHES"
# mirror host sstate-cache to native directory
#SSTATE_MIRRORS = "file://.*/(.*)/(.*) file://native/\\1/\\2 \\n"
LICENSE_FLAGS_WHITELIST = "commercial"
KCONF_AUDIT_LEVEL = "0"
ACCEPT_FSL_EULA = "1"
DISTRO_FEATURES_append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"
DISTRO_FEATURES_BACKFILL_CONSIDERED = "sysvinit"
VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"
# Enable QT Quick controls
PACKAGECONFIG_append_pn-qtbase = " accessibility"
PACKAGECONFIG_append_pn-qtquickcontrols = " accessibility"
# Override uboot splash - This is not working for now, cannot make it work properly.
# BBMASK += "/meta-variscite/recipes-bsp/u-boot/u-boot-splash.bb"
BBMASK += "/meta-vortex-var-som-mx6/recipes-bsp/u-boot/u-boot-splash.bb"
BBMASK += "/meta-vortex-var-som-mx6/recipes-bsp/u-boot/u-boot-variscite.bbappend"
# Do not put logs on a tmpfs
VOLATILE_LOG_DIR = "no"
EOF

# option to enable lite mode for now
if test $setup_l; then
    echo "# delete sources after build" >> conf/local.conf
    echo "INHERIT += \"rm_work\"" >> conf/local.conf
    echo >> conf/local.conf
fi

VORTEX_ROOT_DIR=`readlink -f ../..`

# make a source_me file
if ! [ -e source_me ];then
    echo "#!/bin/sh" >> source_me
    echo "export VORTEX_ROOT=$VORTEX_ROOT_DIR" >> source_me
    echo "export BB_ENV_EXTRAWHITE=\"$BB_ENV_EXTRAWHITE VORTEX_ROOT\"" >> source_me
    echo "cd ${VORTEX_MAESTRO_DIR}/../poky" >> source_me
    echo "set -- $PROJECT_DIR" >> source_me
    echo ". ./oe-init-build-env > /dev/null" >> source_me
    echo "echo \"Back to build project $PROJECT_DIR.\"" >> source_me
fi

prompt_message
cd $PROJECT_DIR
clean_up
