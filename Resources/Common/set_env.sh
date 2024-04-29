#!/bin/bash
set -e

source $SATURN_COMMON/gcc_sh2.env

if [ $INSTALL_YAUL_LIB -eq 1 ]; then
  source $SATURN_YAUL/yaul.env.in
fi

# Run the main container command
exec "$@"
