#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ ! -d $SATURN_SATCONV ]; then
	mkdir -p $SATURN_SATCONV
fi

#
# Download
#
git clone --depth 1 \
    https://git.sr.ht/~ndiddy/satconv/ "$SATURN_TMP/tmp"

#
# build satconv
#

make -f $SATURN_TMP/tmp/Makefile \
		-C $SATURN_TMP/tmp/ $MAKEFLAGS

cp $SATURN_TMP/tmp/{satconv,readme.txt,license.txt} $SATURN_SATCONV

rm -rf "$SATURN_TMP/tmp"

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
