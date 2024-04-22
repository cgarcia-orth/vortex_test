#!/bin/bash


function refresh_services() {
    systemctl daemon-reload
}


function start_service() {
    # Create file
    service_file="/lib/systemd/system/$1.service"
    touch "$service_file"
cat >> "$service_file" <<-EOF
[Unit]
Description="Temporary update service."

[Service]
Type=simple
ExecStart=$2

[Install]
WantedBy=multi-user.target
EOF
    refresh_services
    systemctl start "$1"
}

function clean_service() {
    rm -f "/lib/systemd/system/$1.service"
    refresh_services
}

# Parse input arguments
rollback=false
finalize=false
skip_status_update=false
uninstall_current=false

while [ "$1" != "" ]; do
    case $1 in
        -r | --rollback )           rollback=true
                                    skip_status_update=true
                                    ;;
        -f | --finalize )           finalize=true
                                    ;;
        -s | --skip_status )        skip_status_update=true
                                    ;;
        -c | --uninstall_current )  uninstall_current=true
                                    ;;
    esac
    shift
done

script_name=$(basename "$0")
update_key="vortex-update"
update_status_file="/opt/vortex/update_status"

rollback_folder="/opt/vortex/rollback/"
rollback_application_script="$rollback_folder/ApplicationPackage.sh"
rollback_configuration_script="$rollback_folder/ConfigurationPackage.sh"
rollback_system_script="$rollback_folder/SystemPackage.sh"

update_folder="/opt/vortex/update/"
update_application_script="$update_folder/ApplicationPackage.sh"
update_configuration_script="$update_folder/ConfigurationPackage.sh"
update_system_script="$update_folder/SystemPackage.sh"

current_folder="/opt/vortex/current/"
current_application_script="$current_folder/ApplicationPackage.sh"
current_configuration_script="$current_folder/ConfigurationPackage.sh"
current_system_script="$current_folder/SystemPackage.sh"

remove_folder="/opt/vortex/remove/"
remove_application_script="$remove_folder/ApplicationPackage.sh"
remove_configuration_script="$remove_folder/ConfigurationPackage.sh"
remove_system_script="$remove_folder/SystemPackage.sh"

check_update_file="/opt/vortex/.checkupdate"

# Load previous status
previous_update_status=""
if [ -f "$update_status_file" ]; then
    previous_update_status=$(cat "$update_status_file")
fi

# Allow function to end normaly when stopped
function handle_signal() {
    exit 0
}

trap handle_signal SIGINT SIGTERM

# Check if a verification was successful
if [ $finalize = true ]; then
    # Move update packages into the rollback folder
    mkdir -p "$rollback_folder"
    if [ -f "$current_application_script" ]; then
        logger -t "$script_name" "Moving $current_application_script to $rollback_application_script"
        mv "$current_application_script" "$rollback_application_script"
    fi
    if [ -f "$current_configuration_script" ]; then
        logger -t "$script_name" "Moving $current_configuration_script to $rollback_configuration_script"
        mv "$current_configuration_script" "$rollback_configuration_script"
    fi
    if [ -f "$current_system_script" ]; then
        logger -t "$script_name" "Moving $current_system_script to $rollback_system_script"
        mv "$current_system_script" "$rollback_system_script"
    fi

    # Remove update flag
    rm -f "$check_update_file"
    if [ $skip_status_update = false ]; then
        if [ $previous_update_status != "FAILED" ]; then
            echo "SUCCESS" > "$update_status_file"
        fi
    fi
    
    exit 0
fi

# Make sure update is not already running. Stop on a normal update, kill on a
# rollback
update_services=$(systemctl list-units --type=service --state=active | grep "$update_key")
while IFS= read -r service ;
do
    if ! [[ "$service" == "$update_key"* ]]; then
        continue
    fi
    if [ $rollback = false ]; then
        logger -t "$script_name" "$script_name already running, aborting"
        exit 1
    fi
    service_file_name=$(sed 's/ .*//' <<< "$service")
    service_name=$(sed 's/\..*//' <<< "$service_file_name")
    systemctl stop "$service_name"
    rm -f "/lib/systemd/system/$service_file_name"
done <<< "$maestro_services"
refresh_services

# Make sure we are running from a temporary folder to allow the update of this
# script.
script_folder=$(dirname "$0")
if [[ ! "$script_folder" == "/tmp/"* ]]; then
    tmp_dir=$(mktemp -d -t "$update_key-XXXXXXXXXX")
    dir_name=$(basename "$tmp_dir")
    cp "$0" "$tmp_dir"
    logger -t "$script_name" "Restarting from $tmp_dir"
    exec_str="$tmp_dir/$script_name"
    if [ $rollback = true ]; then
        exec_str="$exec_str -r"
    fi
    if [ $skip_status_update = true ]; then
        exec_str="$exec_str -s"
    fi
    if [ $uninstall_current = true ]; then
        exec_str="$exec_str -c"
    fi 
    logger -t "$script_name" "Launching $exec_str"
    start_service "$dir_name" "$exec_str"
    exit 0
fi
service_name=$(basename "$script_folder")

# Wait for an update check to finish
check_update_running=true
while [ $check_update_running = true ]; do
    if pidof -x "check-update.sh" >/dev/null; then
        logger -t "$script_name" "Waiting for update check to finish"
    else
        check_update_running=false
    fi
    if [ $check_update_running = true ]; then
        sleep 5
    fi
done

# Start install
if [ $skip_status_update = false ]; then
    echo "INSTALLING" > "$update_status_file"
fi

# Prepare verify on next reboot
touch "$check_update_file"

# Stop applications
logger -t "$script_name" "Stopping applications"
systemctl stop maestro-gui
systemctl stop maestro-pro
systemctl stop com-mgr

# Check for rollback
if [ $rollback = true ]; then
    # Remove in case rollback does not have all packages.
    logger -t "$script_name" "Removing update packages in $update_folder before rollback"
    rm -f $update_application_script
    rm -f $update_configuration_script
    rm -f $update_system_script

    # Restore previous packages for install
    mkdir -p "$update_folder"
    if [ -f "$rollback_application_script" ]; then
        logger -t "$script_name" "Moving $rollback_application_script to $update_application_script"
        mv "$rollback_application_script" "$update_application_script"
    fi
    if [ -f "$rollback_configuration_script" ]; then
        logger -t "$script_name" "Moving $rollback_configuration_script to $update_configuration_script"
        mv "$rollback_configuration_script" "$update_configuration_script"
    fi
    if [ -f "$rollback_system_script" ]; then
        logger -t "$script_name" "Moving $rollback_system_script to $update_system_script"
        mv "$rollback_system_script" "$update_system_script"
    fi

    # Rolling back is an automatic fail always
    echo "FAILED" > "$update_status_file"

    # Start normal update
    "/usr/bin/$script_name" -s -c
    clean_service "$service_name"
    exit 0
fi

# Prepare removal of most recent package install
rm -rf "$remove_folder"
mkdir -p "$remove_folder"
if [ $uninstall_current = true ]; then
    application_script="$current_application_script"
    if [ ! -f "$application_script" ]; then
        application_script="$update_application_script"
    fi
    if [ -f "$application_script" ]; then
        cp "$application_script" "$remove_application_script"
    fi
    configuration_script="$current_configuration_script"
    if [ ! -f "$configuration_script" ]; then
        configuration_script="$update_configuration_script"
    fi
    if [ -f "$configuration_script" ]; then
        cp "$configuration_script" "$remove_configuration_script"
    fi
    system_script="$current_system_script"
    if [ ! -f "$system_script" ]; then
        system_script="$update_system_script"
    fi
    if [ -f "$system_script" ]; then
        cp "$system_script" "$remove_system_script"
    fi
else
    if [ -f "$rollback_application_script" ]; then
        cp "$rollback_application_script" "$remove_application_script"
    fi
    if [ -f "$rollback_configuration_script" ]; then
        cp "$rollback_configuration_script" "$remove_configuration_script"
    fi
    if [ -f "$rollback_system_script" ]; then
        cp "$rollback_system_script" "$remove_system_script"
    fi
fi

# Save update packages to current
logger -t "$script_name" "Moving update packages in $current_folder before install"
mkdir -p "$current_folder"
if [ -f "$update_application_script" ]; then
    logger -t "$script_name" "Moving $update_application_script to $current_application_script"
    mv "$update_application_script" "$current_application_script"
    chmod +x "$current_application_script"
fi
if [ -f "$update_configuration_script" ]; then
    logger -t "$script_name" "Moving $update_configuration_script to $current_configuration_script"
    mv "$update_configuration_script" "$current_configuration_script"
    chmod +x "$current_configuration_script"
fi
if [ -f "$update_system_script" ]; then
    logger -t "$script_name" "Moving $update_system_script to $current_system_script"
    mv "$update_system_script" "$current_system_script"
    chmod +x "$current_system_script"
fi

# Uninstall previous packages
logger -t "$script_name" "Uninstalling previous packages"
if [ -f "$remove_application_script" ]; then
    logger -t "$script_name" "Uninstalling $remove_application_script"
    "$remove_application_script" -e
fi
if [ -f "$remove_configuration_script" ]; then
    logger -t "$script_name" "Uninstalling $remove_configuration_script"
    "$remove_configuration_script" -e
fi
if [ -f "$rollback_system_script" ]; then
    logger -t "$script_name" "Uninstalling $remove_system_script"
    "$remove_system_script" -e
fi

# Install new packages
logger -t "$script_name" "Installing new packages"
system_script="$current_system_script"
if [ ! -f "$system_script" ]; then
    system_script="$rollback_system_script"
fi
if [ -f "$system_script" ]; then
    logger -t "$script_name" "Installing $system_script"
    "$system_script"
fi
configuration_script="$current_configuration_script"
if [ ! -f "$configuration_script" ]; then
    configuration_script="$rollback_configuration_script"
fi
if [ -f "$configuration_script" ]; then
    logger -t "$script_name" "Installing $configuration_script"
    if ! "$configuration_script" ; then
        logger -t "$script_name" "$configuration_script install failed, rolling back"
        "/usr/bin/$script_name" -r
        clean_service "$service_name"
        exit 0
    fi
fi
application_script="$current_application_script"
if [ ! -f "$application_script" ]; then
    application_script="$rollback_application_script"
fi
if [ -f "$application_script" ]; then
    logger -t "$script_name" "Installing $application_script"
    "$application_script"
fi

# At this point, we will reboot and start validation after restart
if [ $skip_status_update = false ]; then
    echo "VALIDATING" > "$update_status_file"
fi

# Cleanup service
clean_service "$service_name"

reboot


