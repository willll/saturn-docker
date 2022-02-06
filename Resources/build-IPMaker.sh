#!/bin/bash
set -e

if [ ! -d $SATURN_IPMAKER ]; then
	mkdir -p $SATURN_IPMAKER
fi

cd $SATURN_TMP

#
# build IPMaker
#
mkdir -p $SATURN_TMP/Saturn-SDK-Tool-IPMaker/bin

cmake -S $SATURN_TMP/Saturn-SDK-Tool-IPMaker/ \
			-B $SATURN_TMP/Saturn-SDK-Tool-IPMaker/bin \
			-DCMAKE_INSTALL_PREFIX=$SATURN_IPMAKER
make -f $SATURN_TMP/Saturn-SDK-Tool-IPMaker/bin/Makefile \
			-C $SATURN_TMP/Saturn-SDK-Tool-IPMaker/bin/ && \
make -f $SATURN_TMP/Saturn-SDK-Tool-IPMaker/bin/Makefile \
			-C $SATURN_TMP/Saturn-SDK-Tool-IPMaker/bin/ install

exit 0
