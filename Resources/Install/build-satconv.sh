#!/bin/bash
set -euo pipefail

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

: "${SATURN_SATCONV:?SATURN_SATCONV is not set}"
: "${SATURN_TMP:?SATURN_TMP is not set}"

if ! command -v git >/dev/null 2>&1; then
	echo "git is required but not installed" >&2
	exit 1
fi

if ! command -v make >/dev/null 2>&1; then
	echo "make is required but not installed" >&2
	exit 1
fi

if [ ! -d "$SATURN_SATCONV" ]; then
	mkdir -p "$SATURN_SATCONV"
fi

#
# Download
#
git clone --depth 1 \
    https://git.sr.ht/~ndiddy/satconv/ "$SATURN_TMP/tmp"

#
# build satconv
#

make -f "$SATURN_TMP/tmp/Makefile" \
		-C "$SATURN_TMP/tmp/" ${MAKEFLAGS:-}

cp "$SATURN_TMP/tmp/satconv" "$SATURN_TMP/tmp/readme.txt" "$SATURN_TMP/tmp/license.txt" "$SATURN_SATCONV"

rm -rf "$SATURN_TMP/tmp"

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
