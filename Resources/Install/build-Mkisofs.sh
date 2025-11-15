#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ ! -d $SATURN_CD ]; then
	mkdir -p $SATURN_CD
fi

#
# build schily-tools
#
git clone --depth 1 https://github.com/willll/schily-tools.git "$SATURN_TMP/tmp"

make -f $SATURN_TMP/tmp/Makefile -C $SATURN_TMP/tmp LINKMODE=static $MAKEFLAGS && \
make -f $SATURN_TMP/tmp/Makefile -C $SATURN_TMP/tmp \
      INS_BASE="$SATURN_CD" INS_RBASE="$SATURN_CD" install $MAKEFLAGS

mv $SATURN_CD/bin/* $SATURN_CD && \
rm -rf $SATURN_CD/{bin,ccs,etc,include,lib,sbin,share,xpg4}

rm -rf "$SATURN_TMP/tmp"

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
