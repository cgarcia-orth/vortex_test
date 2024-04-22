#!/bin/bash

####################################################
# Utility method
####################################################

save_pid() {
    PID=$$
    echo ${PID} > $(get_work_dir)/pid.file
}

load_pid() {
    PID=$(cat $(get_work_dir)/pid.file)
    echo ${PID}
}

blink() {
    PID=$(load_pid)
    if [ "${PID}" == "" ]; then
        save_pid
        echo "Blinking until stopped..."
        while true ; do
            echo 1 > /opt/vortex/ios/dmx1_led/value
            echo 1 > /opt/vortex/ios/dmx2_led/value
            echo 1 > /opt/vortex/ios/artnet_led/value
            echo 1 > /opt/vortex/ios/software_led/value
            echo 1 > /opt/vortex/ios/spare_led1/value
            echo 1 > /opt/vortex/ios/spare_led2/value
            echo 1 > /opt/vortex/ios/spare_led3/value
            sleep 0.5
            echo 0 > /opt/vortex/ios/dmx1_led/value
            echo 0 > /opt/vortex/ios/dmx2_led/value
            echo 0 > /opt/vortex/ios/artnet_led/value
            echo 0 > /opt/vortex/ios/software_led/value
            echo 0 > /opt/vortex/ios/spare_led1/value
            echo 0 > /opt/vortex/ios/spare_led2/value
            echo 0 > /opt/vortex/ios/spare_led3/value
            sleep 0.5
        done
    else
        echo "PID file already present, run -stop first before launching again"
    fi
}

solid() {
    PID=$(load_pid)
    if [ "${PID}" == "" ]; then
        save_pid
        echo "Solid until stopped..."
        while true ; do
            echo 1 > /opt/vortex/ios/dmx1_led/value
            echo 1 > /opt/vortex/ios/dmx2_led/value
            echo 1 > /opt/vortex/ios/artnet_led/value
            echo 1 > /opt/vortex/ios/software_led/value
            echo 1 > /opt/vortex/ios/spare_led1/value
            echo 1 > /opt/vortex/ios/spare_led2/value
            echo 1 > /opt/vortex/ios/spare_led3/value
            sleep 0.5
        done
    else
        echo "PID file already present, run -stop first before launching again"
    fi
}

chaser() {
    PID=$(load_pid)
    if [ "${PID}" == "" ]; then
        save_pid
        echo "Chasing until stopped..."
        echo 0 > /opt/vortex/ios/dmx1_led/value
        echo 0 > /opt/vortex/ios/dmx2_led/value
        echo 0 > /opt/vortex/ios/artnet_led/value
        echo 0 > /opt/vortex/ios/software_led/value
        echo 0 > /opt/vortex/ios/spare_led1/value
        echo 0 > /opt/vortex/ios/spare_led2/value
        echo 0 > /opt/vortex/ios/spare_led3/value
        while true ; do
            echo 0 > /opt/vortex/ios/spare_led3/value
            echo 1 > /opt/vortex/ios/dmx1_led/value
            sleep 0.25
            echo 0 > /opt/vortex/ios/dmx1_led/value
            echo 1 > /opt/vortex/ios/dmx2_led/value
            sleep 0.25
            echo 0 > /opt/vortex/ios/dmx2_led/value
            echo 1 > /opt/vortex/ios/artnet_led/value
            sleep 0.25
            echo 0 > /opt/vortex/ios/artnet_led/value
            echo 1 > /opt/vortex/ios/software_led/value
            sleep 0.25
            echo 0 > /opt/vortex/ios/software_led/value
            echo 1 > /opt/vortex/ios/spare_led1/value
            sleep 0.25
            echo 0 > /opt/vortex/ios/spare_led1/value
            echo 1 > /opt/vortex/ios/spare_led2/value
            sleep 0.25
            echo 0 > /opt/vortex/ios/spare_led2/value
            echo 1 > /opt/vortex/ios/spare_led3/value
            sleep 0.25
        done
    else
        echo "PID file already present, run -stop first before launching again"
    fi
}

stop() {
    PID=$(load_pid)
    if [ "${PID}" != "" ]; then
        kill $(load_pid)
        echo 0 > /opt/vortex/ios/dmx1_led/value
        echo 0 > /opt/vortex/ios/dmx2_led/value
        echo 0 > /opt/vortex/ios/artnet_led/value
        echo 0 > /opt/vortex/ios/software_led/value
        echo 0 > /opt/vortex/ios/spare_led1/value
        echo 0 > /opt/vortex/ios/spare_led2/value
        echo 0 > /opt/vortex/ios/spare_led3/value
        rm $(get_work_dir)/pid.file
    else
        echo "No PID file, nothing to stop"
    fi
}

get_work_dir() {
    echo "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
}

####################################################
# Script
####################################################

case $1 in
    "-stop")
        stop
        ;;
    "-blink")
        blink
        ;;
    "-solid")
        solid
        ;;
    "-chaser")
        chaser
        ;;
    *)
        echo "Need to specify an argument, valid arguments are -stop, -blink, -solid or -chaser"
        ;;
esac

