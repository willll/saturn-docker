#!/bin/bash
set -e

if [ $INSTALL_SGL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		echo "Temporary directory not found"
		exit 1
	fi

	if [ "$#" -eq 0 ]; then
		echo "Not enough argument"
		exit 1
	fi

	cd $SATURN_TMP/sgl320

	chmod -R 644 $SATURN_TMP/sgl320

	#
	# convert coff to ELF
	#
	for file in $SATURN_TMP/sgl320/lib/*.{a,o}; do
		$SATURN_ROOT/toolchain/bin/${PROGRAM_PREFIX}objcopy -v -Icoff-sh -Oelf32-sh \
		 		$file
	done

	#
	# Create destination directories
	#
	mkdir -p $1/{cdsrc,lib,include,samples/{sample1,sample2,sample3,sample4},common,share,doc,sys}

	#
	# Copy to destination
	#
	cp -rv $SATURN_TMP/sgl320/cdsrc/* $1/cdsrc
	cp -rv $SATURN_TMP/sgl320/lib/* $1/lib
	cp -rv $SATURN_TMP/sgl320/inc/* $1/include
	cp -rv $SATURN_TMP/sgl320/sample/* $1/samples/sample1
	cp -rv $SATURN_TMP/sgl320/sample2/* $1/samples/sample2
	cp -rv $SATURN_TMP/sgl320/sample3/* $1/samples/sample3
	cp -rv $SATURN_TMP/sgl320/sample4/* $1/samples/sample4
	cp -rv $SATURN_TMP/sgl320/common/* $1/common
	cp -rv $SATURN_TMP/sgl320/share/* $1/share
	cp -rv $SATURN_TMP/sgl320/doc/* $1/doc
	cp -rv $SATURN_TMP/sgl320/sys/* $1/sys
	cp -v $SATURN_TMP/sgl320/version $1/

	# Removed, will come from SBL
	rm $1/include/sl_def.h

	# Clean the code
	find $1 -type f -exec sed -i 's/\o32//g' {} \;

	if [ $INSTALL_SGL_SAMPLES -eq 1 ]; then
		echo "TODO : SGL SAMPLES"
	fi

else
	echo "$(tput setaf 1)No SGL libraries will be built$(tput sgr 0)"

fi

exit 0
