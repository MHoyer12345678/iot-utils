#!/bin/bash

HUB_BUS_NR=1
HUB_DEV_NR=2

PORT=2

# warn at this port because it might be the network device (RPI 3). Doing a power reset
# on the net device might cut your connection off. Warning can be overwritten by
# "--doit" command line option.  
PORT_WARN=1

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo -e "Resets a device powered by a USB port by powering the port off and on again.\n"
    echo -e "Warning: The ethernet device is connected to port $PORT_WARN on RPI3. Reseting"
    echo -e "it might not be what you want to achieve. Overwrite the warning w/ --doit if you"
    echo -e "are sure about a reset on port $PORT_WARN.\n"  
    echo -e "Usage: power-reset-usb-port.sh [PORT_NR] [--doit]"
    echo -e "\t\tPORT: port to reset (default=$PORT)"
    echo -e "\t\t--doit: forces a power reset on port $PORT_WRN"
    exit
fi

if [ "$1" != "" ]; then
    PORT=$1
fi

if [ "$PORT" = "$PORT_WARN" ] && [ "$2" != "--doit" ]; then
    echo -e "An ethernet device used to connect you to your device might be connected"
    echo -e "at port $PORT (e.g. on RPI3). A power reset might disconnect you."
    echo -e "Overwrite this warning w/ --doit, if you still want to do a power reset"
    echo -e "at this port."
    exit
fi

echo "Doing a power reset on port: ${PORT} (${HUB_BUS_NR}:${HUB_DEV_NR})"

echo "Powering off port"
hub-ctrl -b ${HUB_BUS_NR} -d ${HUB_DEV_NR} -P ${PORT} -p 0

sleep 1

echo "Powering on port"
hub-ctrl -b ${HUB_BUS_NR} -d ${HUB_DEV_NR} -P ${PORT} -p 1

echo "Done\n"

