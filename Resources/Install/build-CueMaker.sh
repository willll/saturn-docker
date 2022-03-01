#!/bin/bash
set -e

if [ ! -d $SATURN_CD ]; then
	mkdir -p $SATURN_CD
fi

#
# Download
#
if [ "$#" -ne 1 ]; then
	git clone --depth 1 \
			--recurse-submodules \
	    https://github.com/willll/CueMaker.git "$SATURN_TMP/tmp"
else
	git clone --depth 1 --branch $1 \
			--recurse-submodules \
	    https://github.com/willll/CueMaker.git "$SATURN_TMP/tmp"
fi

#
# build CueMaker
#
mkdir -p $SATURN_TMP/bin

cmake -S $SATURN_TMP/tmp/ \
			-B $SATURN_TMP/bin \
			-DCMAKE_INSTALL_PREFIX=$SATURN_CD

make -f $SATURN_TMP/bin/Makefile \
			-C $SATURN_TMP/bin/ && \
make -f $SATURN_TMP/bin/Makefile \
			-C $SATURN_TMP/bin/ install

rm -rf "$SATURN_TMP/tmp"

exit 0
