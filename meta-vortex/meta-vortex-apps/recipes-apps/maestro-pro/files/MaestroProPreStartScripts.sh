#!/bin/bash
script_name=$(basename "$0")

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

settingEthernetUp() {

    dhcpClientOn=$1

    logger -t "$script_name" "settingEthernetUp"
    # make sure no invalid address is set
    ip addr flush dev eth0

    #create config for connmand
    echo [service_vortex] > /tmp/vortex.config
    echo Type=ethernet >> /tmp/vortex.config
    echo Name=Wired >> /tmp/vortex.config
    echo -n MAC= >> /tmp/vortex.config
    cat /sys/class/net/eth0/address >> /tmp/vortex.config
    if [[ $dhcpClientOn = "on" ]]; then
        echo IPv4=dhcp >> /tmp/vortex.config
    else
        echo IPv4=99.99.99.1/255.255.255.0/99.99.99.1 >> /tmp/vortex.config
    fi
    mv /tmp/vortex.config  /var/lib/connman/vortex.config

    # make sure no invalid address is set
    if [[ $dhcpClientOn != "on" ]]; then #use dhcp
        systemctl start dhcpd
    fi

}

settingLteUp() {

    gsmApn=$1
    gsmUser=$2
    gsmPassword=$3
    gsmPin=$4
    
    logger -t "$script_name" "settingLteUp"

    if [[ -z $gsmUser && -z $gsmPassword ]]; then
        auth="noauth"
    fi


    cp /opt/vortex/quectel/quectel-chat-connect-template /etc/ppp/peers/quectel-chat-connect
    sed -i "s/%apn%/$gsmApn/g" /etc/ppp/peers/quectel-chat-connect

    cp /opt/vortex/quectel/quectel-ppp-template /etc/ppp/peers/quectel-ppp
    sed -i "s/%user%/$gsmUser/g" /etc/ppp/peers/quectel-ppp
    sed -i "s/%password%/$gsmPassword/g" /etc/ppp/peers/quectel-ppp
    sed -i "s/%auth%/$auth/g" /etc/ppp/peers/quectel-ppp

    # enable cell modem
    echo 1 > /opt/vortex/ios/3g_perst/value
    echo 1 > /opt/vortex/ios/3g_w_disable/value

    # start driver
    systemctl start pppd.path
}

main () {

    logger -t "$script_name" "testing influx to listen on port 8096"   
    netstat -ano | grep 8096 | grep LISTEN > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then 
      logger -t "$script_name" "influx not ready, restarting prestart"    
      exit 1
    fi 

    logger -t "$script_name" "start setting interfaces"

    # grab values from the settings or set at default:
    wlanInterfaceType=$(xmlValueFor "//settings/user-settings/wlanInterfaceType/@value" "eth")
    gsmApn=$(xmlValueFor "//settings/user-settings/gsmApn/@value"  "")
    gsmUser=$(xmlValueFor "//settings/user-settings/gsmUser/@value"  "")
    gsmPassword=$(xmlValueFor "//settings/user-settings/gsmPassword/@value"  "")
    gsmPin=$(xmlValueFor "//settings/user-settings/gsmPin/@value" "")
    dhcpClientOn=$(xmlValueFor "//settings/user-settings/dhcpClientOn/@value"  "on")

    # we always want ethernet to be online
    settingEthernetUp $dhcpClientOn

    # default: wan is set to ethernet
    if [[ $wlanInterfaceType = "cell" ]]; then
        settingLteUp $gsmApn $gsmUser $gsmPassword $gsmPin
    elif [[ $wlanInterfaceType = "usb" ]]; then
        #allow android phones to connect as kernel module is blacklisted
        modprobe rndis_host
    fi
    
       
    logger -t "$script_name" "end setting interfaces"
}

main
