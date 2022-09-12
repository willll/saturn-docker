#!/bin/bash
set -e

if [ $INSTALL_SGL_SAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		echo "Temporary directory not found"
		exit 1
	fi

	if [ "$#" -eq 0 ]; then
		echo "Not enough argument"
		exit 1
	fi

	cd $SATURN_TMP

	#
	# build samples
	#
	mkdir -p ${SATURN_TMP}/sgl302/sample/bin
	cmake -S $SATURN_TMP/sgl302/sample/ -B $SATURN_TMP/sgl302/sample/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$1
	make -f $SATURN_TMP/sgl302/sample/bin/Makefile -C $SATURN_TMP/sgl302/sample/bin/ && \
	 		make -f $SATURN_TMP/sgl302/sample/bin/Makefile -C $SATURN_TMP/sgl302/sample/bin/ install

else
	echo "$(tput setaf 1)No SGL samples will be built$(tput sgr 0)"

fi

exit 0
