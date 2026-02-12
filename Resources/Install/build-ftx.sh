#!/bin/bash
set -euo pipefail

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

: "${SATURN_FTX:?SATURN_FTX is not set}"
: "${SATURN_TMP:?SATURN_TMP is not set}"

if ! command -v git >/dev/null 2>&1; then
	echo "git is required but not installed" >&2
	exit 1
fi

if ! command -v cmake >/dev/null 2>&1; then
	echo "cmake is required but not installed" >&2
	exit 1
fi

if ! command -v make >/dev/null 2>&1; then
	echo "make is required but not installed" >&2
	exit 1
fi

if [ ! -d "$SATURN_FTX" ]; then
	mkdir -p "$SATURN_FTX"
fi

#
# Download
#
if [ "$#" -ge 1 ] && [ -n "${1:-}" ]; then
	git clone --depth 1 --branch "$1" https://github.com/willll/ftx.git "$SATURN_TMP/tmp"
else
	git clone --depth 1 https://github.com/willll/ftx.git "$SATURN_TMP/tmp"
fi

#
# Build ftx
#
mkdir -p "$SATURN_TMP/tmp/build"
cmake -S "$SATURN_TMP/tmp" -B "$SATURN_TMP/tmp/build" -DCMAKE_BUILD_TYPE=Release
cmake --build "$SATURN_TMP/tmp/build" -- ${MAKEFLAGS:-}

if [ ! -f "$SATURN_TMP/tmp/build/ftx" ]; then
	echo "ftx binary not found at $SATURN_TMP/tmp/build/ftx" >&2
	exit 1
fi

cp "$SATURN_TMP/tmp/build/ftx" "$SATURN_FTX"

rm -rf "$SATURN_TMP/tmp"

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
