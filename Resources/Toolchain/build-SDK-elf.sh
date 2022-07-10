#!/bin/bash
set -e

export PROGRAM_PREFIX=sh-elf-
export TARGETMACH=sh-elf
export OBJFORMAT=ELF

cd "${BUILD_FOLDER}"

. ./versions.sh && ./build-elf.sh

exit 0
