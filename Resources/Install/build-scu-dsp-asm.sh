#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ ! -d $SATURN_SCU_DSP ]; then
	mkdir -p $SATURN_SCU_DSP
fi

#
# build scu-dsp-asm
#
git clone --depth 1 https://github.com/buhman/scu-dsp-asm.git "$SATURN_TMP/tmp"

make -f $SATURN_TMP/tmp/Makefile -C $SATURN_TMP/tmp  $MAKEFLAGS

mv $SATURN_TMP/tmp/{scu-dsp-asm,scu-dsp-dis,gen,test} $SATURN_SCU_DSP/ 

rm -rf "$SATURN_TMP/tmp"

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
