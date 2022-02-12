#!/bin/bash
set -e

if [ $INSTALL_SBL_EXAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		echo "$SATURN_TMP does not exist !"
		exit 1
	fi

	cd $SATURN_TMP

	curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/19nKbK6eFHh1cliyFYsBv-P2IGrWTc0Ci/view?usp=sharing"& \
	curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	 ./cookie-file1`&id=19nKbK6eFHh1cliyFYsBv-P2IGrWTc0Ci" -o $SATURN_TMP/examples.zip

	if [ ! -f "$SATURN_TMP/examples.zip" ]; then
	    echo "$SATURN_TMP/examples.zip not downloaded."
	    exit 1
	fi

	unzip $SATURN_TMP/examples.zip -d $SATURN_TMP/examples

fi

exit 0
