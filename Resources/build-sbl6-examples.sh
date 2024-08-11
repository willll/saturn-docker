#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ $INSTALL_SBL_EXAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_TMP/examples ]; then
		echo "$SATURN_TMP/examples does not exist !"
		exit 1
	fi

	cd $SATURN_TMP

	# Clean the code
	find $SATURN_TMP/examples -type f -exec sed -i 's/\o32//g' {} \;

	# Patch the code
	patch -p0 -d $SATURN_TMP < sbl6_examples.patch

	# Inject Build System
	cp --verbose -rf $SATURN_TMP/sbl6_/examples/* $SATURN_TMP/examples/

	#
	# build samples
	#
	mkdir -p $SATURN_TMP/examples/bin
	cmake -S $SATURN_TMP/examples/ -B $SATURN_TMP/examples/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SBL \
			-DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit 1
	make -f $SATURN_TMP/examples/bin/Makefile -C $SATURN_TMP/examples/bin/ VERBOSE=1 $MAKEFLAGS || exit 1
	make -f $SATURN_TMP/examples/bin/Makefile -C $SATURN_TMP/examples/bin/ install $MAKEFLAGS || exit 1

else
	echo "$(tput setaf 1)No SBL examples will be built$(tput sgr 0)"

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
