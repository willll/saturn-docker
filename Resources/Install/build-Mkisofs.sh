#!/bin/bash
set -euo pipefail

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

: "${SATURN_CD:?SATURN_CD is not set}"
: "${SATURN_TMP:?SATURN_TMP is not set}"

if ! command -v git >/dev/null 2>&1; then
	echo "git is required but not installed" >&2
	exit 1
fi

if ! command -v make >/dev/null 2>&1; then
	echo "make is required but not installed" >&2
	exit 1
fi

if [ ! -d "$SATURN_CD" ]; then
	mkdir -p "$SATURN_CD"
fi

#
# build schily-tools
#
git clone --depth 1 https://github.com/willll/schily-tools.git "$SATURN_TMP/tmp"

make -f "$SATURN_TMP/tmp/Makefile" -C "$SATURN_TMP/tmp" LINKMODE=static ${MAKEFLAGS:-} && \
make -f "$SATURN_TMP/tmp/Makefile" -C "$SATURN_TMP/tmp" \
      INS_BASE="$SATURN_CD" INS_RBASE="$SATURN_CD" install ${MAKEFLAGS:-}

if [ -d "$SATURN_CD/bin" ]; then
	mv "$SATURN_CD/bin/"* "$SATURN_CD"
	rm -rf "$SATURN_CD/bin" "$SATURN_CD/ccs" "$SATURN_CD/etc" "$SATURN_CD/include" \
		"$SATURN_CD/lib" "$SATURN_CD/sbin" "$SATURN_CD/share" "$SATURN_CD/xpg4"
fi

rm -rf "$SATURN_TMP/tmp"

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
