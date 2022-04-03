#!/bin/bash

if [ $UID -ne 0 ]; then
    echo "Please run as root ..."
    exit 1
fi

rm /sbin/init || exit 1
ln -s /sbin/init-devel.sh /sbin/init || exit 1

echo "Developer mode enabled. /sbin/init now pointing to /sbin/init-devel.sh."

exit 0
