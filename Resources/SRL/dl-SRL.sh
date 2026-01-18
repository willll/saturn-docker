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

  	git clone --recurse-submodules -j2 -b $SRL_LIB_TAG --depth 1 https://github.com/ReyeMe/SaturnRingLib.git "$SATURN_SRL"

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
