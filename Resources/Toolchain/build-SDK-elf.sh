#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

export PROGRAM_PREFIX=sh-elf-
export TARGETMACH=sh-elf
export OBJFORMAT=ELF
export GCC_BOOTSTRAP="--disable-bootstrap"
export ROOTDIR="${BUILD_FOLDER}"
#export ENABLE_DOWNLOAD_CACHE=0
#export ENABLE_STATIC_BUILD=0
#export ENABLE_BOOTSTRAP=0


cd "${BUILD_FOLDER}"

. ./versions.sh && ./build-elf.sh

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
