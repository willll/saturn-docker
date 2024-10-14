#!/bin/bash
set -e

# Start SSHD
service ssh restart

. /opt/saturn/common//tools.env
. $SATURN_COMMON/gcc_sh2.env

# Run the main container command
exec "$@"
