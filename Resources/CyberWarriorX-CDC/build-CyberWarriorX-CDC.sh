#!/bin/bash
set -e

if [ $INSTALL_CYBERWARRIORX_CDC_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		echo "$SATURN_TMP does not exist !"
		exit 1
	fi

	if [ "$#" -eq 0 ]; then
		echo "Not enough argument"
		exit 1
	fi

	cd $SATURN_TMP/cdcrepv100rc1

	chmod -R 644 $SATURN_TMP/cdcrepv100rc1

	#
	# Create destination directories
	#
	mkdir -p $1/{lib,include,src}

	#
	# Copy to destination
	#
	cp --verbose -rf $SATURN_TMP/cdcrepv100rc1/*.h $1/include
	cp --verbose -rf $SATURN_TMP/cdcrepv100rc1/*.c $1/src

	#
	# build segalib
	#
	mkdir -p $SATURN_TMP/cdcrepv100rc1/bin
	cmake -S $1/ -B $SATURN_TMP/cdcrepv100rc1/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$1/
	make -f $SATURN_TMP/cdcrepv100rc1/bin/Makefile -C $SATURN_TMP/cdcrepv100rc1/bin/ VERBOSE=1 && \
	 		make -f $SATURN_TMP/cdcrepv100rc1/bin/Makefile -C $SATURN_TMP/cdcrepv100rc1/bin/ install

else
	echo "$(tput setaf 1)No CyberWarriorX-CDC libraries will be built$(tput sgr 0)"

fi

exit 0
