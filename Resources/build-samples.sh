#!/bin/bash
if [ ! -d $SATURN_SAMPLES ]; then
	mkdir -p $SATURN_SAMPLES
fi

# Settings
build_samples=1
make_ISO=0

cd $SATURN_SAMPLES

#
# build samples
#
if [ $build_samples -eq 1 ]; then
	mkdir -p $SATURN_SAMPLES/bin
	cmake -S $SATURN_SAMPLES/ -B $SATURN_SAMPLES/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SAMPLES
	make -f $SATURN_SAMPLES/bin/Makefile -C $SATURN_SAMPLES/bin/ && \
	 		make -f $SATURN_SAMPLES/bin/Makefile -C $SATURN_SAMPLES/bin/ install
fi

#
# make ISOs
#
if [ $make_ISO -eq 1 ]; then
	mkdir -p ${SATURN_SAMPLES}/CD
fi
