#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

export PROGRAM_PREFIX=sh-elf-
export TARGETMACH=sh-elf
export OBJFORMAT=ELF
export ROOTDIR="${BUILD_FOLDER}"

cd "${BUILD_FOLDER}"

. ./versions.sh && ./build-elf.sh

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
