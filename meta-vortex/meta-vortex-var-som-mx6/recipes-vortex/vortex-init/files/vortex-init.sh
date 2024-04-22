#!/bin/bash

# Setup CAN
not_found=true
try_count=0
while ${not_found}; do
    sleep 1
    for interface in $(/sbin/ifconfig -a | sed 's/[ \t].*//;/^\(lo\|\)$/d')
    do
        if [ ${interface} = "can0" ] ; then
            /sbin/ip link set can0 type can restart-ms 100
            /sbin/ip link set can0 up type can bitrate 125000
            /sbin/ip link set txqueuelen 128 dev can0
            not_found=false
        fi
    done
    try_count=$((try_count+1))
    if [ ${try_count} -eq 60 ]; then
        echo "Failed to initialize CAN interface"
        break
    fi
done

# IOs Init
mkdir -p /opt/vortex/ios

echo 145 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio145/direction
echo 0 > /sys/class/gpio/gpio145/value
ln -s -f /sys/class/gpio/gpio145 /opt/vortex/ios/dmx1_led

echo 140 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio140/direction
echo 0 > /sys/class/gpio/gpio140/value
ln -s -f /sys/class/gpio/gpio140 /opt/vortex/ios/dmx2_led

echo 144 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio144/direction
echo 0 > /sys/class/gpio/gpio144/value
ln -s -f /sys/class/gpio/gpio144 /opt/vortex/ios/artnet_led

echo 141 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio141/direction
echo 0 > /sys/class/gpio/gpio141/value
ln -s -f /sys/class/gpio/gpio141 /opt/vortex/ios/software_led

echo 142 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio142/direction
echo 0 > /sys/class/gpio/gpio142/value
ln -s -f /sys/class/gpio/gpio142 /opt/vortex/ios/uart4_dir

echo 143 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio143/direction
echo 0 > /sys/class/gpio/gpio143/value
ln -s -f /sys/class/gpio/gpio143 /opt/vortex/ios/uart5_dir

echo 43 > /sys/class/gpio/export
echo "in" > /sys/class/gpio/gpio43/direction
ln -s -f /sys/class/gpio/gpio43 /opt/vortex/ios/early_pwr_fail

echo 85 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio85/direction
echo 1 > /sys/class/gpio/gpio85/value
ln -s -f /sys/class/gpio/gpio85 /opt/vortex/ios/sys_working_led

echo 161 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio161/direction
echo 0 > /sys/class/gpio/gpio161/value
ln -s -f /sys/class/gpio/gpio161 /opt/vortex/ios/3g_dtr

echo 149 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio149/direction
echo 0 > /sys/class/gpio/gpio149/value
ln -s -f /sys/class/gpio/gpio149 /opt/vortex/ios/3g_w_disable

echo 148 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio148/direction
echo 0 > /sys/class/gpio/gpio148/value
ln -s -f /sys/class/gpio/gpio148 /opt/vortex/ios/3g_perst

echo 160 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio160/direction
echo 0 > /sys/class/gpio/gpio160/value
ln -s -f /sys/class/gpio/gpio160 /opt/vortex/ios/spare_led1

echo 162 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio162/direction
echo 0 > /sys/class/gpio/gpio162/value
ln -s -f /sys/class/gpio/gpio162 /opt/vortex/ios/spare_led2

echo 146 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio146/direction
echo 0 > /sys/class/gpio/gpio146/value
ln -s -f /sys/class/gpio/gpio146 /opt/vortex/ios/spare_led3

echo 2 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio2/direction
echo 0 > /sys/class/gpio/gpio2/value
ln -s -f /sys/class/gpio/gpio2 /opt/vortex/ios/mcu_reset

#Stop NTP
echo "Disable NTP"
systemctl stop systemd-timesyncd
systemctl disable systemd-timesyncd

#Stop usbmuxd
echo "Disable usbmuxd"
systemctl stop usbmuxd
systemctl disable usbmuxd

#use RTC time
echo "Set date with hardware clock."
hwclock --hctosys

#configure galcore clock throttling
echo 4 > /sys/bus/platform/drivers/galcore/gpu3DMinClock


