#!/bin/bash
set -e

if [ $INSTALL_YAUL_LIB -eq 1 ]; then
  source $SATURN_YAUL/yaul.env.in
fi

# Run the main container command
exec "$@"
