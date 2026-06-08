#!/bin/bash
set -euo pipefail

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
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

curl -fSL -o "$SATURN_MEDNAFEN/mednafen" https://github.com/willll/mednafenSSDev/releases/download/v.01/mednafen

mkdir -p "$SATURN_MEDNAFEN/firmware"

gdown 1miIZbAytFFRGlmL8H2gM8Hs__A3AC23P -O "$SATURN_MEDNAFEN/firmware/mpr-17933.bin"
         
gdown 1YvFP-7DCGq4AVd2Xo-HAaUQmikV6ufVT -O "$SATURN_MEDNAFEN/firmware/sega_101.bin"
      
if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
