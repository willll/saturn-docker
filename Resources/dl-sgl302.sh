#!/bin/bash
set -e


if [ $INSTALL_SGL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	#
	# download
	#
	curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/13yg-wiATGdVlVRseqCD-JBfltau5A1L-/view?usp=sharing"& \
	curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	 ./cookie-file1`&id=13yg-wiATGdVlVRseqCD-JBfltau5A1L-" -o $SATURN_TMP/sgl302.zip

	if [ ! -f "$SATURN_TMP/sgl302.zip" ]; then
	    echo "$SATURN_TMP/sgl302.zip not downloaded."
	    exit 1
	fi

	curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/1YBSYfESM2KlHC_kXf4kK2eSPFgmuRCvh/view?usp=sharing"& \
	curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	./cookie-file1`&id=1YBSYfESM2KlHC_kXf4kK2eSPFgmuRCvh" -o $SATURN_TMP/libsgl_real_elf_from_vbt.zip

	if [ ! -f "$SATURN_TMP/libsgl_real_elf_from_vbt.zip" ]; then
			echo "$SATURN_TMP/libsgl_real_elf_from_vbt.zip not downloaded."
			#exit 1
	fi

	#
	# Extract
	#
	unzip $SATURN_TMP/sgl302.zip -d $SATURN_TMP

	if [ -f "$SATURN_TMP/libsgl_real_elf_from_vbt.zip" ]; then
		unzip $SATURN_TMP/libsgl_real_elf_from_vbt.zip -d $SATURN_TMP/sgl302/lib
	fi
fi

exit 0
