#!/bin/bash
set -euo pipefail

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

: "${SATURN_MEDNAFEN:?SATURN_MEDNAFEN is not set}"

if [ ! -d "$SATURN_MEDNAFEN" ]; then
	mkdir -p "$SATURN_MEDNAFEN"
fi

if ! command -v curl >/dev/null 2>&1; then
	echo "curl is required but not installed" >&2
	exit 1
fi

if ! command -v unzip >/dev/null 2>&1; then
	echo "unzip is required but not installed" >&2
	exit 1
fi

#
# build mednafen
#
curl -fSL -o "$SATURN_MEDNAFEN/Mednafen.Linux.zip" https://github.com/willll/mednafenSSDev/releases/download/initial/Mednafen.Linux.zip

unzip -o "$SATURN_MEDNAFEN/Mednafen.Linux.zip" -d "$SATURN_MEDNAFEN"

rm -f "$SATURN_MEDNAFEN/Mednafen.Linux.zip"

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
