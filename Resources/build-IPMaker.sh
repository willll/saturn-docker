#!/bin/bash
set -e

if [ ! -d $SATURN_IPMAKER ]; then
	mkdir -p $SATURN_IPMAKER
fi

#
# build IPMaker
#
mkdir -p $SATURN_TMP/bin

cmake -S $SATURN_TMP/ \
			-B $SATURN_TMP/bin \
			-DCMAKE_INSTALL_PREFIX=$SATURN_ROOT
make -f $SATURN_TMP/bin/Makefile \
			-C $SATURN_TMP/bin/ && \
make -f $SATURN_TMP/bin/Makefile \
			-C $SATURN_TMP/bin/ install

exit 0
