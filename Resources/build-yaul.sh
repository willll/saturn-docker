#!/bin/bash
set -e

if [ $INSTALL_YAUL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_YAUL ]; then
		echo "$SATURN_YAUL does not exist !"
		exit 1
	fi

	cd $SATURN_TMP

	#
	# build yaul in release
	#
  make install-release

	#
	# Install yaul tools
	#
  make install-tools


fi

exit 0
