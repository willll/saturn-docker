#!/bin/bash
set -e

if [ ! -d $SATURN_CD ]; then
	mkdir -p $SATURN_CD
fi

#
# build schily-tools
#
git clone https://github.com/willll/schily-tools.git "$SATURN_TMP/tmp"

make $MAKEFLAGS -f $SATURN_TMP/tmp/Makefile -C $SATURN_TMP/tmp LINKMODE=static && \
make $MAKEFLAGS -f $SATURN_TMP/tmp/Makefile -C $SATURN_TMP/tmp \
      INS_BASE="$SATURN_CD" INS_RBASE="$SATURN_CD" install

mv $SATURN_CD/bin/* $SATURN_CD && \
rm -rf $SATURN_CD/{bin,ccs,etc,include,lib,sbin,share,xpg4}

rm -rf "$SATURN_TMP/tmp"

exit 0
