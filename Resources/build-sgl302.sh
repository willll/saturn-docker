#!/bin/bash
set -e

if [ ! -d $SATURN_TMP ]; then
	echo "Temporary directory not found"
	exit 1
fi

if [ "$#" -eq 0 ]; then
	echo "Not enough argument"
	exit 1
fi

cd $SATURN_TMP/sgl302

chmod -R 644 $SATURN_TMP/sgl302

#
# convert coff to ELF
#
for file in $SATURN_TMP/sgl302/lib/*.{a,o}; do
	$SATURN_ROOT/toolchain/bin/${PROGRAM_PREFIX}objcopy -v -Icoff-sh -Oelf32-sh \
	 		$file
done

#
# Create destination directories
#
mkdir -p $1/{lib,include,samples/{sample1,sample2},demos,doc,sddrv}

#
# Copy to destination
#
cp -rv $SATURN_TMP/sgl302/lib/* $1/lib
cp -rv $SATURN_TMP/sgl302/inc/* $1/include
cp -rv $SATURN_TMP/sgl302/sample/* $1/samples/sample1
cp -rv $SATURN_TMP/sgl302/sample2/* $1/samples/sample2
cp -rv $SATURN_TMP/sgl302/demos/* $1/demos
cp -rv $SATURN_TMP/sgl302/doc/* $1/doc
cp -rv $SATURN_TMP/sgl302/sddrv/* $1/sddrv
