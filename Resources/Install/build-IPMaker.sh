#!/bin/bash
set -euo pipefail

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

: "${SATURN_IPMAKER:?SATURN_IPMAKER is not set}"
: "${SATURN_TMP:?SATURN_TMP is not set}"
: "${SATURN_ROOT:?SATURN_ROOT is not set}"

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

if [ "$#" -lt 1 ] || [ -z "${1:-}" ]; then
	echo "IPMaker git branch is required as the first argument" >&2
	exit 1
fi

if [ ! -d "$SATURN_IPMAKER" ]; then
	mkdir -p "$SATURN_IPMAKER"
fi

#
# Download
#
git clone --depth 1 --branch "$1" \
    https://github.com/willll/Saturn-SDK-Tool-IPMaker.git "$SATURN_TMP/tmp"

#
# build IPMaker
#
mkdir -p "$SATURN_TMP/bin"

cmake -S "$SATURN_TMP/tmp/" \
			-B "$SATURN_TMP/bin" \
			-DCMAKE_INSTALL_PREFIX="$SATURN_ROOT"
make -f "$SATURN_TMP/bin/Makefile" \
			-C "$SATURN_TMP/bin/" ${MAKEFLAGS:-} && \
make -f "$SATURN_TMP/bin/Makefile" \
			-C "$SATURN_TMP/bin/" install ${MAKEFLAGS:-}

rm -rf "$SATURN_TMP/tmp"

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
