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

	#curl -s -o nul -c ./cookie-file2 -s -L "https://drive.google.com/file/d/1CMDCnjfggL7PQRb-VLPrk0qtTesBgHtd/view?usp=sharing"& \
	#curl -Lb ./cookie-file2 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	#./cookie-file2`&id=1CMDCnjfggL7PQRb-VLPrk0qtTesBgHtd/" -o "$SATURN_TMP/libsgl_real_elf_from_vbt.zip"

	gdown 1CMDCnjfggL7PQRb-VLPrk0qtTesBgHtd -O "$SATURN_TMP/libsgl_real_elf_from_vbt.zip"


	if [ ! -f "$SATURN_TMP/libsgl_real_elf_from_vbt.zip" ]; then
			echo "$SATURN_TMP/libsgl_real_elf_from_vbt.zip not downloaded."
			#exit 1
	fi

	#
	# Extract
	#
	unzip $SATURN_TMP/sgl302.zip -d $SATURN_TMP

	if [ -f "$SATURN_TMP/libsgl_real_elf_from_vbt.zip" ]; then
		ls -lh "$SATURN_TMP/libsgl_real_elf_from_vbt.zip"

		unzip "$SATURN_TMP/libsgl_real_elf_from_vbt.zip" -d $SATURN_TMP/sgl302
		retVal=$?
		if [ $retVal -ne 0 ]; then
		    echo "unzip unzip $SATURN_TMP/libsgl_real_elf_from_vbt.zip failed !"
				exit $retVal
		fi
	fi
fi

exit 0
