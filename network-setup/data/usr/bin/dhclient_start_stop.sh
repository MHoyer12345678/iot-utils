#!/bin/bash

echo P1: $1 P2 $2

if [ "$2" = "CONNECTED" ]; then
	dhclient -4 -cf /etc/dhcp/dhclient.conf wlan0
	touch /run/wifi-connected
elif [ "$2" = "DISCONNECTED" ]; then
	killall dhclient
	rm /run/wifi-connected
fi
      	
