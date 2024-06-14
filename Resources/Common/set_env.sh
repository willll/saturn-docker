#!/bin/bash
set -e

. $SATURN_COMMON/gcc_sh2.env

if [ $INSTALL_YAUL_LIB -eq 1 ]; then
  . $SATURN_YAUL/yaul.env.in
fi

env | grep _ >> /etc/environment

egrep -Eo ' [a-zA-Z_-]+=' $SATURN_COMMON/gcc_sh2.env | cut -c 2- | rev | cut -c 2- | rev |\
while read line; do env | grep "$line" >> /etc/environment ; done
