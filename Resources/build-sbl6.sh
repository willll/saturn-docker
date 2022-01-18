#!/bin/bash
set -e

if [ ! -d $SATURN_TMP ]; then
	mkdir -p $SATURN_TMP
fi

# Settings
build_segalib=1
build_segasmp=1

cd $SATURN_TMP

# diff -crB --binary sbl6/ sbl6_patch/ > sbl6.patch
# Patch the code
patch -p0 -d $SATURN_TMP < sbl6.patch

# Clean the code
find $SATURN_TMP -type f -exec sed -i 's/\o32//g' {} \;
rm $SATURN_TMP/sbl6/segalib/spr/sega_spr.h
#rm $SATURN_TMP/sbl6/segasmp/dual/sega_spr.h
#rm $SATURN_TMP/sbl6/segasmp/dual/sega_xpt.h

# Inject Build System
cp -r $SATURN_TMP/sbl6_/* $SATURN_TMP/sbl6/

#
# build segalib
#
if [ $build_segalib -eq 1 ]; then
	mkdir -p $SATURN_TMP/sbl6/segalib/bin
	cmake -S $SATURN_TMP/sbl6/segalib/ -B $SATURN_TMP/sbl6/segalib/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SBL
	make -f $SATURN_TMP/sbl6/segalib/bin/Makefile -C $SATURN_TMP/sbl6/segalib/bin/ && \
	 		make -f $SATURN_TMP/sbl6/segalib/bin/Makefile -C $SATURN_TMP/sbl6/segalib/bin/ install
fi

#
# convert sega_sgl to ELF
#
#$SATURN_ROOT/toolchain/bin/${PROGRAM_PREFIX}objcopy -v -Icoff-sh -Oelf32-sh \
#		$SATURN_TMP/sbl6/segalib/lib/sega_sgl.a
cp -v $SATURN_TMP/sbl6/segalib/lib/sega_sgl.a $SATURN_SBL/segalib/lib

#
# build segasmp
#
if [ $build_segasmp -eq 1 ]; then
	mkdir -p ${SATURN_TMP}/sbl6/segasmp/bin
	mkdir -p ${SATURN_SBL}/segasmp/bup/CD
	cmake -S $SATURN_TMP/sbl6/segasmp/ -B $SATURN_TMP/sbl6/segasmp/bin/ \
			-DCMAKE_TOOLCHAIN_FILE=$SATURN_CMAKE/sega_saturn.cmake \
			-DCMAKE_INSTALL_PREFIX=$SATURN_SBL
	make -f $SATURN_TMP/sbl6/segasmp/bin/Makefile -C $SATURN_TMP/sbl6/segasmp/bin/ VERBOSE=1 && \
	 		make -f $SATURN_TMP/sbl6/segasmp/bin/Makefile -C $SATURN_TMP/sbl6/segasmp/bin/ install
fi

#
# Create ISO file
#
if [ $build_segasmp -eq 1 ]; then
	# BUP
	cp $SATURN_SBL/segasmp/bup/bup.bin $SATURN_SBL/segasmp/bup/CD/0.bin
	cp $SATURN_CD/{CPY.txt,BIB.txt,ABS.txt} $SATURN_SBL/segasmp/bup/CD/
	cp $SATURN_TMP/sbl6/segasmp/bup/IP.BIN $SATURN_SBL/segasmp/bup/CD/
	cp $SATURN_TMP/sbl6/segasmp/bup/bup.cue $SATURN_SBL/segasmp/bup/
	cp $SATURN_TMP/sbl6/segasmp/bup/cd_included_files.txt $SATURN_SBL/segasmp/bup/CD/FILES.txt
	cp $SATURN_TMP/sbl6/segasmp/bup/cd_excludes_files.txt $SATURN_SBL/segasmp/bup/CD/EXCLUDES.txt

	cd $SATURN_SBL/segasmp/bup/CD/ && \
	$SATURN_CD/create_cd.sh "bup" "Kenshiro" "Raoh" \
					"IP.BIN" \
	 				$SATURN_SBL/segasmp/bup/bup.iso
fi
