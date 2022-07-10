#!/bin/bash
set -e

export PROGRAM_PREFIX=sh-coff-
export TARGETMACH=sh-coff
export OBJFORMAT=COFF

cd "${BUILD_FOLDER}"

. ./versions.sh && ./build-coff.sh

exit 0
