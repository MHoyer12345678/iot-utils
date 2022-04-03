#!/bin/bash

SWITCH_TO_DEVEL_OS_TRIGGER_FN=/boot/switch-into-develop-os.trigger

if [ $UID -ne 0 ]; then
    echo "Please run as root ..."
    exit 1
fi

mount /dev/mmcblk0p1 /boot
touch $SWITCH_TO_DEVEL_OS_TRIGGER_FN || exit 1
umount /boot

echo "Trigger set to boot into developer OS on next reboot."

init_path=$(realpath /sbin/init)

if [ $init_path != "/usr/sbin/init-devel.sh" ]; then
    echo "Developer mode is not enabled. To boot into developer OS please enable developer mode using enable-developer-mode.sh."
fi

exit 0
