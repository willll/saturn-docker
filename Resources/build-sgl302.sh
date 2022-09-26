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

	# Patch the code
	patch -p0 -d $SATURN_TMP < $SATURN_TMP/sgl302.patch

	#
	# Create destination directories
	#
	mkdir -p $1/{common,lib,include,samples/{sample1,sample2},demos,doc,sddrv}


	#
	# convert coff to ELF
	#
	find $SATURN_TMP/sgl302/lib/ \
	 -name "*.[a,o]" \
	 -type f \
	 -exec "$SATURN_ROOT/toolchain/bin/${PROGRAM_PREFIX}objcopy" -Icoff-sh -Oelf32-sh {} \;

	#
 	# Install libsgl patched version
 	#
	if [ -f "$SATURN_TMP/sgl302/libsgl_real_elf.a" ]; then
		mv -f "$SATURN_TMP/sgl302/libsgl_real_elf.a" "$SATURN_TMP/sgl302/lib/libsgl.a"
	fi

	# Clean the code
	find $SATURN_TMP/sgl302 \
		! -name "*.[a,o,lib]" \
		-type f \
		-exec sed -i 's/\o32//g' {} \;

	#
	# Inject Build System
	#
	#cp --verbose -rf $SATURN_TMP/sgl_/* $SATURN_TMP/sgl302/


	#
	# Copy to destination
	#
	cp -r $SATURN_TMP/sgl302/lib/* $1/lib
	cp -r $SATURN_TMP/sgl302/inc/* $1/include
	cp -r $SATURN_TMP/sgl302/sample/* $1/samples/sample1
	cp -r $SATURN_TMP/sgl302/sample2/* $1/samples/sample2
	cp -r $SATURN_TMP/sgl302/demos/* $1/demos
	cp -r $SATURN_TMP/sgl302/doc/* $1/doc
	cp -r $SATURN_TMP/sgl302/sddrv/* $1/sddrv
	cp -r $SATURN_TMP/sgl302/sample/common/* $1/common
	cp -r $SATURN_TMP/sgl302/sample/scroll/ $1/demos/biplane

	if [ $INSTALL_SBL_LIB -eq 1 ]; then
		# Removed, will come from SBL
		rm $1/include/sl_def.h
	fi

else
	echo "$(tput setaf 1)No SGL libraries will be built$(tput sgr 0)"

fi

exit 0
