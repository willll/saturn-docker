#!/bin/bash
set -e

if [ $INSTALL_SBL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/1wqc7bngPUOv_0hgyuIyCTq8l4jc_azSN/view?usp=sharing"& \
	curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	 ./cookie-file1`&id=1wqc7bngPUOv_0hgyuIyCTq8l4jc_azSN" -o $SATURN_TMP/sdk_10j.zip

	if [ ! -f "$SATURN_TMP/sdk_10j.zip" ]; then
	    echo "$SATURN_TMP/sdk_10j.zip not downloaded."
	    exit 1
	fi

	unzip $SATURN_TMP/sdk_10j.zip -d $SATURN_TMP

fi

exit 0
