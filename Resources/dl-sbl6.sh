#!/bin/bash
set -e

if [ $INSTALL_SBL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	gdown 1Nz-aCEe9U3Rxc9QUn3IA6rMP4GY_EwSZ -O "$SATURN_TMP/sbl6.zip"

	if [ ! -f "$SATURN_TMP/sbl6.zip" ]; then
	    echo "$SATURN_TMP/sbl6.zip not downloaded."
	    exit 1
	fi

	unzip $SATURN_TMP/sbl6.zip -d $SATURN_TMP

fi

exit 0
