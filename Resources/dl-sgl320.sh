#!/bin/bash
set -e


if [ $INSTALL_SGL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	#
	# download
	#

  curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/160C4hJADdFUzrgNcSN_MG7MrSH9Phgjq/view?usp=sharing"& \
	curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	./cookie-file1`&id=160C4hJADdFUzrgNcSN_MG7MrSH9Phgjq" -o $SATURN_TMP/sgl320.zip

	if [ ! -f "$SATURN_TMP/sgl320.zip" ]; then
	    echo "$SATURN_TMP/sgl320.zip not downloaded."
	    exit 1
	fi

	#
	# Extract
	#
	unzip $SATURN_TMP/sgl320.zip -d $SATURN_TMP

fi

exit 0
