#!/bin/bash

SWITCH_TO_DEVEL_OS_TRIGGER_FN=/boot/switch-into-develop-os.trigger
DEVELOPMENT_IMG_PARTITION=/dev/mmcblk0p4

mount /dev/mmcblk0p1 /boot

if [ ! -f $SWITCH_TO_DEVEL_OS_TRIGGER_FN ]; then
    echo "Booting into production image ..."
    exec /lib/systemd/systemd
fi

# remove trigger to boot next time into productive OS (recovery if devel image fails)
rm $SWITCH_TO_DEVEL_OS_TRIGGER_FN

umount /boot

# prepare to boot into partition with development image
mount -o ro $DEVELOPMENT_IMG_PARTITION /mnt
umount -f /
exec /sbin/chroot /mnt /sbin/init

