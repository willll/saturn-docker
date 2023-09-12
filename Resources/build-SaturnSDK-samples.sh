#!/bin/bash
set -e

if [ $INSTALL_SATURNSDK_SAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_SAMPLES ]; then
		mkdir -p $SATURN_SAMPLES
	fi

	cd $SATURN_SAMPLES

	#
	# build samples
	#
	mkdir -p $SATURN_SAMPLES/bin
	cmake -S $SATURN_SAMPLES/ -B $SATURN_SAMPLES/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SAMPLES
	make $MAKEFLAGS -f $SATURN_SAMPLES/bin/Makefile -C $SATURN_SAMPLES/bin/ && \
		make $MAKEFLAGS -f $SATURN_SAMPLES/bin/Makefile -C $SATURN_SAMPLES/bin/ install

else
	echo "$(tput setaf 1)No SDK samples will be built$(tput sgr 0)"

fi

exit 0
