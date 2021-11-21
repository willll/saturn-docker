#!/bin/bash
if [ ! -d $SATURN_TMP ]; then
	mkdir -p $SATURN_TMP
fi

cd $SATURN_TMP

# diff -crB --binary sbl6/ sbl6_patch/ > sbl6.patch
# Patch the code
patch -p0 -d $SATURN_TMP < sbl6.patch

# Clean the code
find $SATURN_TMP -type f -exec sed -i 's/\o32//g' {} \;
rm $SATURN_TMP/sbl6/segalib/spr/sega_spr.h


# Inject Build System
cp -r $SATURN_TMP/sbl6_/* $SATURN_TMP/sbl6/


# build segalib
mkdir -p $SATURN_TMP/sbl6/segalib/bin
cmake -S $SATURN_TMP/sbl6/segalib/ -B $SATURN_TMP/sbl6/segalib/bin/ \
		-DCMAKE_INSTALL_PREFIX=$SATURN_SBL
make -f $SATURN_TMP/sbl6/segalib/bin/Makefile -C $SATURN_TMP/sbl6/segalib/bin/ && \
 		make -f $SATURN_TMP/sbl6/segalib/bin/Makefile -C $SATURN_TMP/sbl6/segalib/bin/ install

# build segasmp
mkdir -p ${SATURN_TMP}/sbl6/segasmp/bin
cmake -S $SATURN_TMP/sbl6/segasmp/ -B $SATURN_TMP/sbl6/segasmp/bin/ \
		-DCMAKE_INSTALL_PREFIX=$SATURN_SBL
#make -f $SATURN_TMP/sbl6/segasmp/bin/Makefile -C $SATURN_TMP/sbl6/segasmp/bin/ 
