#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

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
	else
	    echo "$SATURN_TMP/sgl302.zip downloaded."
		#chmod -R 755 "$SATURN_TMP/sgl302.zip"
	fi

	#
	# download SGL 3.02 patched version
	#
	gdown 1CMDCnjfggL7PQRb-VLPrk0qtTesBgHtd -O "$SATURN_TMP/libsgl_real_elf_from_vbt.zip"

	if [ ! -f "$SATURN_TMP/libsgl_real_elf_from_vbt.zip" ]; then
			echo "$SATURN_TMP/libsgl_real_elf_from_vbt.zip not downloaded."
			exit 1
	else
			echo "$SATURN_TMP/libsgl_real_elf_from_vbt.zip downloaded."
			#chmod -R 755 "$SATURN_TMP/libsgl_real_elf_from_vbt.zip"
	fi

	#
	# Extract
	#
	umask 664 && unzip $SATURN_TMP/sgl302.zip -d $SATURN_TMP

	EXTRA_FILES="libsgl_real_elf_from_vbt.zip"

	for F in $EXTRA_FILES
	do
		if [ -f "$SATURN_TMP/$F" ]; then
			ls -lh "$SATURN_TMP/$F"

			umask 644 && unzip "$SATURN_TMP/$F" -d $SATURN_TMP/sgl302
			retVal=$?
			if [ $retVal -ne 0 ]; then
					echo "unzip $SATURN_TMP/$F failed !"
					exit $retVal
			else
				echo "unzip $SATURN_TMP/$F succeeded."
			fi
		fi
	done

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
