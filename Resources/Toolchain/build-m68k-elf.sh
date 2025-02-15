#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ $INSTALL_M68K_GCC -eq 1 ]; then

	export GCC_BOOTSTRAP="--disable-bootstrap"
	export PROGRAM_PREFIX=m68k-
	export TARGETMACH=m68k-elf
	export OBJFORMAT=ELF
	export ROOTDIR="${BUILD_FOLDER}"

	cd "${BUILD_FOLDER}"

	. ./versions.sh && ./build.sh

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
