#!/bin/bash
set -e

# Start SSHD, if root
if [[ "$EUID" == 0 ]]; then
    service ssh restart
else
    echo "Not running as root, skipping SSHD start"
fi

. /opt/saturn/common//tools.env
. $SATURN_COMMON/gcc_sh2.env

# Run the main container command
exec "$@"
