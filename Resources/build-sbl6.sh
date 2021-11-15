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

# build segalib
