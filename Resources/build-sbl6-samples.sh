#!/bin/bash
set -e

if [ $INSTALL_SBL_SAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	cd $SATURN_TMP

	if [ $INSTALL_SBL_LIB -eq 0 ]; then
		# diff -crB --binary sbl6/ sbl6_patch/ > sbl6.patch
		# Patch the code
		patch -p0 -d $SATURN_TMP < sbl6.patch
		# Inject Build System
		cp --verbose -rf $SATURN_TMP/sbl6_/* $SATURN_TMP/sbl6/
	fi

	#
	# build segasmp
	#
	mkdir -p ${SATURN_TMP}/sbl6/segasmp/bin
	cmake -S $SATURN_TMP/sbl6/segasmp/ -B $SATURN_TMP/sbl6/segasmp/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SBL
	make -f $SATURN_TMP/sbl6/segasmp/bin/Makefile -C $SATURN_TMP/sbl6/segasmp/bin/ && \
	 		make -f $SATURN_TMP/sbl6/segasmp/bin/Makefile -C $SATURN_TMP/sbl6/segasmp/bin/ install

else
	echo "$(tput setaf 1)No SBL Samples will be built$(tput sgr 0)"

fi

exit 0
