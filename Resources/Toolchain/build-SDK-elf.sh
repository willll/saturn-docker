#!/bin/bash
set -e

export PROGRAM_PREFIX=sh-elf-
export TARGETMACH=sh-elf
export OBJFORMAT=ELF
export GCC_BOOTSTRAP="--disable-bootstrap"
export ENABLE_DOWNLOAD_CACHE=0

cd "${BUILD_FOLDER}"

. ./versions.sh && ./build-elf.sh

exit 0
