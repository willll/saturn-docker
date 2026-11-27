#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

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

	# Patch the code

	patch -p0 -d $SATURN_TMP < $SATURN_TMP/sgl302.patch --binary

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

	# Clean the code: drop the DOS end of file (0x1A) of the text files only
	# (it is data in the binaries: "*.[a,o,lib]" matched one character, the sound driver, sound and model data lost bytes)
	find $SATURN_TMP/sgl302 \
		-type f \
		-exec env LC_ALL=C grep -Iq . {} \; \
		-exec sed -i 's/\o32//g' {} \;

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

	if [[ $INSTALL_SBL_LIB != 1 ]]; then
		# Removed, will come from SBL
		rm $1/include/sl_def.h
	fi

else
	echo "$(tput setaf 1)No SGL libraries will be built$(tput sgr 0)"

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
