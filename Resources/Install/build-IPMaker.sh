#!/bin/bash
set -e

if [ ! -d $SATURN_IPMAKER ]; then
	mkdir -p $SATURN_IPMAKER
fi

#
# Download
#
git clone --depth 1 --branch $1 \
    https://github.com/willll/Saturn-SDK-Tool-IPMaker.git "$SATURN_TMP/tmp"

#
# build IPMaker
#
mkdir -p $SATURN_TMP/bin

cmake -S $SATURN_TMP/tmp/ \
			-B $SATURN_TMP/bin \
			-DCMAKE_INSTALL_PREFIX=$SATURN_ROOT
make -f $SATURN_TMP/bin/Makefile \
			-C $SATURN_TMP/bin/ $MAKEFLAGS && \
make -f $SATURN_TMP/bin/Makefile \
			-C $SATURN_TMP/bin/ install $MAKEFLAGS

rm -rf "$SATURN_TMP/tmp"

exit 0
