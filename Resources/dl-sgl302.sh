#!/bin/bash
set -e


if [ $INSTALL_SGL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	#
	# download
	#
	gdown 13yg-wiATGdVlVRseqCD-JBfltau5A1L- -O "$SATURN_TMP/sgl302.zip"

	if [ ! -f "$SATURN_TMP/sgl302.zip" ]; then
	    echo "$SATURN_TMP/sgl302.zip not downloaded."
	    exit 1
	fi

	#
	# download extra samples from SGL 3.20
	#

	gdown 1zFb9g3x6AZfDvP1dW7PWIXkFqT-boWzl -O "$SATURN_TMP/sgl320_sample4.zip"

	if [ ! -f "$SATURN_TMP/sgl320_sample4.zip" ]; then
	    echo "$SATURN_TMP/sgl320_sample4.zip not downloaded."
	    #exit 1
	fi

	#
	# download extra shared code from SGL 3.20
	#

	gdown 1qFQZP7qu8I-rEXGxegeHAtgoLnNJ2jkh -O "$SATURN_TMP/sgl320_share.zip"

	if [ ! -f "$SATURN_TMP/sgl320_share.zip" ]; then
			echo "$SATURN_TMP/sgl320_share.zip not downloaded."
			#exit 1
	fi

	#
	# download SGL 3.02 patched version
	#
	gdown 1CMDCnjfggL7PQRb-VLPrk0qtTesBgHtd -O "$SATURN_TMP/libsgl_real_elf_from_vbt.zip"

	if [ ! -f "$SATURN_TMP/libsgl_real_elf_from_vbt.zip" ]; then
			echo "$SATURN_TMP/libsgl_real_elf_from_vbt.zip not downloaded."
			#exit 1
	fi

	#
	# Extract
	#
	unzip $SATURN_TMP/sgl302.zip -d $SATURN_TMP

	EXTRA_FILES="libsgl_real_elf_from_vbt.zip
							sgl320_sample4.zip
							sgl320_share.zip"

	for F in $EXTRA_FILES
	do
		if [ -f "$SATURN_TMP/$F" ]; then
			ls -lh "$SATURN_TMP/$F"

			unzip "$SATURN_TMP/$F" -d $SATURN_TMP/sgl302
			retVal=$?
			if [ $retVal -ne 0 ]; then
					echo "unzip $SATURN_TMP/$F failed !"
					exit $retVal
			fi
		fi
	done

fi

exit 0
