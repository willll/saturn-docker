#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

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
	# Inject Build System
	#
	cp --verbose -rf $SATURN_TMP/sgl_/* $SATURN_TMP/sgl302/
	cp -rv $SATURN_TMP/sgl302/sample/* $1/samples/sample1
	cp -rv $SATURN_TMP/sgl302/sample2/* $1/samples/sample2
	cp -rv $SATURN_TMP/sgl302/sample4/* $1/samples/sample4
	cp -rv $SATURN_TMP/sgl302/demos/* $1/demos

	#
	# build sample1
	#
	mkdir -p ${SATURN_TMP}/sgl302/sample/bin
	cmake -S $SATURN_TMP/sgl302/sample/ -B $SATURN_TMP/sgl302/sample/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$1 \
			-DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit 1
	make -f $SATURN_TMP/sgl302/sample/bin/Makefile -C $SATURN_TMP/sgl302/sample/bin/ $MAKEFLAGS && \
		make -f $SATURN_TMP/sgl302/sample/bin/Makefile -C $SATURN_TMP/sgl302/sample/bin/ install $MAKEFLAGS

	#
	# build sample2
	#
	mkdir -p ${SATURN_TMP}/sgl302/sample2/bin
	cmake -S $SATURN_TMP/sgl302/sample2/ -B $SATURN_TMP/sgl302/sample2/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$1 \
			-DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit 1
	make -f $SATURN_TMP/sgl302/sample2/bin/Makefile -C $SATURN_TMP/sgl302/sample2/bin/ && \
	 		make -f $SATURN_TMP/sgl302/sample2/bin/Makefile -C $SATURN_TMP/sgl302/sample2/bin/ install

	#
	# build sample4
	#
	mkdir -p ${SATURN_TMP}/sgl302/sample4/bin
	cmake -S $SATURN_TMP/sgl302/sample4/ -B $SATURN_TMP/sgl302/sample4/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$1 \
			-DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit 1
	make -f $SATURN_TMP/sgl302/sample4/bin/Makefile -C $SATURN_TMP/sgl302/sample4/bin/ && \
	 		make -f $SATURN_TMP/sgl302/sample4/bin/Makefile -C $SATURN_TMP/sgl302/sample4/bin/ install

	#
	# build demos
	#
	mkdir -p ${SATURN_TMP}/sgl302/demos/bin
	cmake -S $SATURN_TMP/sgl302/demos/ -B $SATURN_TMP/sgl302/demos/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$1 \
			-DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit 1
	make -f $SATURN_TMP/sgl302/demos/bin/Makefile -C $SATURN_TMP/sgl302/demos/bin/ && \
	 		make -f $SATURN_TMP/sgl302/demos/bin/Makefile -C $SATURN_TMP/sgl302/demos/bin/ install

else
	echo "$(tput setaf 1)No SGL samples will be built$(tput sgr 0)"

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
