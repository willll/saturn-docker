#!/bin/bash
set -e

if [ $INSTALL_SBL_EXAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_TMP/examples ]; then
		echo "$SATURN_TMP/examples does not exist !"
		exit 1
	fi

	cd $SATURN_TMP/examples

	#
	# build samples
	#
	mkdir -p $SATURN_TMP/examples/bin
	cmake -S $SATURN_TMP/examples/ -B $SATURN_SAMPLES/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SAMPLES/examples
	make -f $SATURN_TMP/examples/bin/Makefile -C $SATURN_TMP/examples/bin/ && \
	 		make -f $SATURN_TMP/examples/bin/Makefile -C $SATURN_TMP/examples/bin/ install

fi

exit 0
