#!/bin/bash
set -e

# Start SSHD
service ssh restart

# Run the main container command
exec "$@"
