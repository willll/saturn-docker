#!/bin/bash
set -e

git clone --depth 1 --branch gcc_$GCC_VERSION \
    https://github.com/willll/Saturn-SDK-GCC-SH2.git "$BUILD_FOLDER"

chmod 777 "$BUILD_FOLDER"/*.sh

dos2unix "$BUILD_FOLDER"/*

chmod +x "$BUILD_FOLDER"/*.sh

exit 0
