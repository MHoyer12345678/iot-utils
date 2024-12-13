#!/bin/bash

if [ -x /usr/bin/first-boot-script.sh ]; then
    /usr/bin/first-boot-script.sh
else
    echo "First boot script /usr/bin/first-boot-script.sh not found (or not executable)."
fi

# mark firt boot as done
touch /var/lib/first-boot-done.state
