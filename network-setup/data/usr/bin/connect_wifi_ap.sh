#!/bin/bash

touch /run/resolv.conf
/sbin/wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant-wlan0.conf
/sbin/wpa_cli -i wlan0 -a /usr/bin/dhclient_start_stop.sh
