#!/bin/bash
set -e

if [ $INSTALL_JO_ENGINE_LIB -eq 1 ]; then

	if [ ! -d $SATURN_JOENGINE ]; then
		echo "$SATURN_JOENGINE does not exist !"
		exit 1
	fi

	cd $SATURN_TMP

	#
	# build yaul in release
	#
  

fi

exit 0
