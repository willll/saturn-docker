#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

if [ $INSTALL_SBL63_LIB -eq 1 ]; then

	# SBL 6.3: SBL 6.2 with sources rebuilt for a modern sh-elf GCC, plus SGL 3.20; lowercase like SBL 6.01 (the zip holds an sbl63/ folder)
	gdown 12fRjuaR3qrycJPKPg3I6GI8dk4l64z87 -O "$SATURN_TMP/sbl63.zip"

	if [ ! -f "$SATURN_TMP/sbl63.zip" ]; then
	    echo "$SATURN_TMP/sbl63.zip not downloaded."
	    exit 1
	fi

	unzip -q "$SATURN_TMP/sbl63.zip" -d "$SATURN_TMP"
	mv "$SATURN_TMP/sbl63" "$SATURN_SBL63"
	rm "$SATURN_TMP/sbl63.zip"

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
