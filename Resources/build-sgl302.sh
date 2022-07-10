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

	cd $SATURN_TMP/sgl302

	chmod -R 644 $SATURN_TMP/sgl302

	#
	# Create destination directories
	#
	mkdir -p $1/{lib,include,samples/{sample1,sample2},demos,doc,sddrv}
	
	if [ $INSTALL_COFF_LIB -eq 1 ]; then
			mkdir -p $1/lib/coff
			cp -rv $SATURN_TMP/sgl320/lib/* $1/lib/coff
	fi

	#
	# convert coff to ELF
	#
	for file in $SATURN_TMP/sgl302/lib/*.{a,o}; do
		$SATURN_ROOT/toolchain/bin/${PROGRAM_PREFIX}objcopy -v -Icoff-sh -Oelf32-sh \
		 		$file
	done

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
