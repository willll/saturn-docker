#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

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
			-DCMAKE_INSTALL_PREFIX=$SATURN_SBL \
			-DCMAKE_BUILD_TYPE=$BUILD_TYPE -G Ninja || exit 1
	ninja -f $SATURN_TMP/sbl6/segasmp/bin/build.ninja \
					-C $SATURN_TMP/sbl6/segasmp/bin/ $MAKEFLAGS || exit 1
	ninja -f $SATURN_TMP/sbl6/segasmp/bin/build.ninja \
					-C $SATURN_TMP/sbl6/segasmp/bin/ install $MAKEFLAGS || exit 1

else
	echo "$(tput setaf 1)No SBL Samples will be built$(tput sgr 0)"

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
