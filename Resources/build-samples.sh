#!/bin/bash
if [ ! -d $SATURN_SAMPLES ]; then
	mkdir -p $SATURN_SAMPLES
fi

cd $SATURN_SAMPLES

# build segalib
mkdir -p $SATURN_SAMPLES/bin
cmake -S $SATURN_SAMPLES/ -B $SATURN_SAMPLES/bin/ \
		-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
		-DCMAKE_INSTALL_PREFIX=$SATURN_SAMPLES
make -f $SATURN_SAMPLES/bin/Makefile -C $SATURN_SAMPLES/bin/ && \
 		make -f $SATURN_SAMPLES/bin/Makefile -C $SATURN_SAMPLES/bin/ install
