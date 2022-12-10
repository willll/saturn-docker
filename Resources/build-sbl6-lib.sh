#!/bin/bash
set -e

if [ $INSTALL_SBL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	cd $SATURN_TMP

	# diff -crB --binary sbl6/ sbl6_patch/ > sbl6.patch
	# Patch the code
	patch -p0 -d $SATURN_TMP < sbl6.patch

	# Clean the code
	find $SATURN_TMP \
		! -name "*.[o,a,lib,bin]" \
		-type f \
		-exec sed -i 's/\o32//g' {} \;

	rm $SATURN_TMP/sbl6/segalib/spr/sega_spr.h
	#rm $SATURN_TMP/sbl6/segalib/include/sl_def.h
	rm $SATURN_TMP/sbl6/segalib/include/sgl.h
	#rm $SATURN_TMP/sbl6/segasmp/dual/sega_spr.h
	#rm $SATURN_TMP/sbl6/segasmp/dual/sega_xpt.h

	# Inject Build System
	cp --verbose -rf $SATURN_TMP/sbl6_/* $SATURN_TMP/sbl6/

	#
	# build segalib
	#
	mkdir -p $SATURN_TMP/sbl6/segalib/bin
	cmake -S $SATURN_TMP/sbl6/segalib/ -B $SATURN_TMP/sbl6/segalib/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SBL
	make -f $SATURN_TMP/sbl6/segalib/bin/Makefile -C $SATURN_TMP/sbl6/segalib/bin/ VERBOSE=1 && \
	 		make -f $SATURN_TMP/sbl6/segalib/bin/Makefile -C $SATURN_TMP/sbl6/segalib/bin/ install

	#
	# build sega_sat
	#
	mkdir -p $SATURN_TMP/sbl6/segalib/sat/bin
	cmake -S $SATURN_TMP/sbl6/segalib/sat -B $SATURN_TMP/sbl6/segalib/sat/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SBL
	make -f $SATURN_TMP/sbl6/segalib/sat/bin/Makefile -C $SATURN_TMP/sbl6/segalib/sat/bin/ && \
	 		make -f $SATURN_TMP/sbl6/segalib/sat/bin/Makefile -C $SATURN_TMP/sbl6/segalib/sat/bin/ install

	#
	# convert sega_sgl and sega_adp to ELF
	#
	for filename in sega_sgl.a sega_sys.a ;
	do
		$SATURN_ROOT/toolchain/bin/${PROGRAM_PREFIX}objcopy -v -Icoff-sh -Oelf32-sh \
				$SATURN_TMP/sbl6/segalib/lib/$filename
		cp -v $SATURN_TMP/sbl6/segalib/lib/$filename $SATURN_SBL/segalib/lib
  done

	cp -v $SATURN_TMP/sbl6/segalib/lib/sega_adp.a $SATURN_SBL/segalib/lib
	#cp -v $SATURN_TMP/sbl6/segalib/lib/sega_sys.a $SATURN_SBL/segalib/lib
	#cp -v $SATURN_TMP/sbl6/segalib/lib/sega_per.a $SATURN_SBL/segalib/lib

else
	echo "$(tput setaf 1)No SBL libraries will be built$(tput sgr 0)"

fi

exit 0
