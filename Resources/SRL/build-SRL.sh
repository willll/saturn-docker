#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

if [ $INSTALL_SRL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_SRL ]; then
		echo "$SATURN_SRL does not exist !"
		exit 1
	fi

	cd $SATURN_SRL/Samples

	#
	# build Samples
	#
	./buildall.bat

else
	echo "$(tput setaf 1)No SRL library will be built$(tput sgr 0)"

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
