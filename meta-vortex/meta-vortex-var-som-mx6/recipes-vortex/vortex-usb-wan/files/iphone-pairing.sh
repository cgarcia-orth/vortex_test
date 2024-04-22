#!/bin/bash
script_name=$(basename "$0")

function handle_signal() {
    exit 0
}

trap handle_signal SIGINT SIGTERM

function xmlValueFor () {
    # $1: xpath of the field
    # $2: default value
    settings_file="/opt/vortex/configuration/settings.xml"
    if [ -f "$settings_file" ]; then
        attribute_found=$(xmllint --xpath "boolean($1)" "$settings_file")
        xmllint_result=$?
        if [ $xmllint_result -eq 0 ]; then
            if [ "$attribute_found" = "true" ]; then
                echo $(xmllint --xpath "string($1)" "$settings_file")
            else
                echo $2
            fi
        else
           echo $2
        fi
    else
        echo $2
    fi
}


function start_pairing() {
    /usr/sbin/usbmuxd -X
    sleep 2
    logger -t "$script_name" "Start usbmuxd"
    /usr/sbin/usbmuxd --user root
    sleep 2

    pairingOK=1
    while [[ -c "${DEVNAME}" ]]
    do
        if [[ ${pairingOK} -ne 0 ]]; then
            timeout 10 /usr/bin/idevicepair pair
            pairingOK=$?
            if [[ ${pairingOK} -eq 0 ]]; then
                logger -t "$script_name" "pairing complete"
                exit 0
            else 
                logger -t "$script_name" "pairing error ${pairingOK} - retrying"
            fi
        fi
        sleep 1
    done
   
    logger -t "$script_name" "iphone device stopped"
}

function stop_pairing() {
    logger -t "$script_name" "Stop usbmuxd"
    /usr/sbin/usbmuxd -X
}

function main () {
	logger -t "$script_name" " -- ${ACTION} -- ${DEVNAME}"
	
  # grab values from the settings or set at default:
  wlanInterfaceType=$(xmlValueFor "//settings/user-settings/wlanInterfaceType/@value" "")

  if [[ $wlanInterfaceType != "usb" ]]; then
      logger -t "$script_name" " wan type not selected to usb, iphone can not be paired"
      exit 0
  fi

	if [[ ! -c "${DEVNAME}" ]]; then
		logger -t "$script_name" "No device exists - stop pairing"
	fi

	if [ "${ACTION}" = "add" ]; then 
		start_pairing
	elif [ "${ACTION}" = "remove" ]; then
		stop_pairing 
	else 
		logger -t "$script_name" "requires add or remove ACTION in environment"
	fi
}

main

