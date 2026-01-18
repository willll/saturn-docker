#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

git clone --depth 1 --branch gcc_$GCC_VERSION \
    https://github.com/willll/Saturn-SDK-GCC-SH2.git "$BUILD_FOLDER"

chmod 777 "$BUILD_FOLDER"/*.sh

dos2unix "$BUILD_FOLDER"/*

chmod +x "$BUILD_FOLDER"/*.sh

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
