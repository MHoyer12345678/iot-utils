#!/bin/bash

if [ $UID -ne 0 ]; then
    echo "Please run as root ..."
    exit 1
fi

rm /sbin/init || exit 1
ln -s /lib/systemd/systemd /sbin/init || exit 1

echo "Developer mode disabled. /sbin/init now pointing to /lib/systemd/systemd."

exit 0
