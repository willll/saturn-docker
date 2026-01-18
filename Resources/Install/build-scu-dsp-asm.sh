#!/bin/bash
set -euo pipefail

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

: "${SATURN_SCU_DSP:?SATURN_SCU_DSP is not set}"
: "${SATURN_TMP:?SATURN_TMP is not set}"

if ! command -v git >/dev/null 2>&1; then
	echo "git is required but not installed" >&2
	exit 1
fi

if ! command -v make >/dev/null 2>&1; then
	echo "make is required but not installed" >&2
	exit 1
fi

if [ ! -d "$SATURN_SCU_DSP" ]; then
	mkdir -p "$SATURN_SCU_DSP"
fi

#
# build scu-dsp-asm
#
git clone --depth 1 https://github.com/buhman/scu-dsp-asm.git "$SATURN_TMP/tmp"

make -f "$SATURN_TMP/tmp/Makefile" -C "$SATURN_TMP/tmp" ${MAKEFLAGS:-}

mv "$SATURN_TMP/tmp/scu-dsp-asm" "$SATURN_TMP/tmp/scu-dsp-dis" \
	"$SATURN_TMP/tmp/gen" "$SATURN_TMP/tmp/test" "$SATURN_SCU_DSP/"

rm -rf "$SATURN_TMP/tmp"

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
