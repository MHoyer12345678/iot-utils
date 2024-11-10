#!/bin/bash

#script enables network package forwarding from wlan0 to eth0. This can be used to enable internet access on development targets

#enable ip forwarding
echo "1" > /proc/sys/net/ipv4/ip_forward

#add masquerade rule to post processing in nat table
iptables -t nat -A POSTROUTING -o wlp0s20f3 -j MASQUERADE
