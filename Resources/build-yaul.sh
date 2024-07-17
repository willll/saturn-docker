#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ $INSTALL_YAUL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_YAUL ]; then
		echo "$SATURN_YAUL does not exist !"
		exit 1
	fi

	cd $SATURN_TMP/yaul

	#
	# build yaul in release
	#
  make install-release $MAKEFLAGS

	#
	# Install yaul tools
	#
  make install-tools $MAKEFLAGS


else
	echo "$(tput setaf 1)No YAUL library will be built$(tput sgr 0)"

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
