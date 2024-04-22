#!/bin/bash
# Keep variable context after a pipe
shopt -s lastpipe
script_name=$(basename "$0")
update_script="/usr/bin/vortex-update.sh"


function handle_signal() {
    exit 0
}

trap handle_signal SIGINT SIGTERM


function count_errors() {
    rcount="0"
    journalctl -u $1.service -b --no-pager | while read journal_log ; do
        if [[ $journal_log == *"systemd[1]: $1.service: Main process exited"* ]]; then
            rcount=$((rcount+1))
        fi
    done
    echo $rcount
}


# Validate if check should run
check_update_file="/opt/vortex/.checkupdate"
if [ ! -f "$check_update_file" ]; then
    logger -t "$script_name" "No update check required"
    exit 0
fi

# Allow some time for service to run
sleep 120

# Check status and reset count of all services
services=("maestro-pro" "maestro-gui" "com-mgr")
for service in "${services[@]}"
do
    # Check if service is currently running
    service_running=false
    for i in {1..10}
    do
        systemctl is-active --quiet ${service}.service
        is_active=$?
        if [ $is_active -eq 0 ]; then
            service_running=true
            break

        else
            logger -t "$script_name" "Service $service not running during attempt $i"
        fi
    done
    if [ "$service_running" = false ]; then
        logger -t "$script_name" "Service $service not running, rolling back"
        "$update_script" -r
        exit 0
    fi

    # Check if service crashed or was rebooted by watchdog
    rcount="$(count_errors $service)"
    if [ $rcount -gt 0 ]; then
        logger -t "$script_name" "Service $service was unexpectedly terminated, rolling back"
        "$update_script" -r
        exit 0
    fi    
done

logger -t "$script_name" "Update check complete, no problems encountered"
"$update_script" -f

