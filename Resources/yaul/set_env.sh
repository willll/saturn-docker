#!/bin/bash
set -e

. $SATURN_COMMON/gcc_sh2.env

if [ $INSTALL_YAUL_LIB -eq 1 ]; then
  . $SATURN_YAUL/yaul.env.in
fi

# Run the main container command
exec "$@"
