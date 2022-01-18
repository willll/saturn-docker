#!/bin/bash
set -e

if [ $INSTALL_SBL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	cd $SATURN_TMP

	#curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/19nKbK6eFHh1cliyFYsBv-P2IGrWTc0Ci/view?usp=sharing"& \
	#curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	# ./cookie-file1`&id=19nKbK6eFHh1cliyFYsBv-P2IGrWTc0Ci" -o $SATURN_TMP/examples.zip

	#if [ ! -f "$SATURN_TMP/examples.zip" ]; then
	#    echo "$SATURN_TMP/examples.zip not downloaded."
	#    exit 1
	#fi

	curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/1Nz-aCEe9U3Rxc9QUn3IA6rMP4GY_EwSZ/view?usp=sharing"& \
	curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	 ./cookie-file1`&id=1Nz-aCEe9U3Rxc9QUn3IA6rMP4GY_EwSZ" -o $SATURN_TMP/sbl6.zip

	if [ ! -f "$SATURN_TMP/sbl6.zip" ]; then
	    echo "$SATURN_TMP/sbl6.zip not downloaded."
	    exit 1
	fi

	#curl -s -o nul -c ./cookie-file1 -s -L "https://drive.google.com/file/d/1wqc7bngPUOv_0hgyuIyCTq8l4jc_azSN/view?usp=sharing"& \
	#curl -Lb ./cookie-file1 "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' \
	# ./cookie-file1`&id=1wqc7bngPUOv_0hgyuIyCTq8l4jc_azSN" -o $SATURN_TMP/sdk_10j.zip

	#if [ ! -f "$SATURN_TMP/sdk_10j.zip" ]; then
	#    echo "$SATURN_TMP/sdk_10j.zip not downloaded."
	#    exit 1
	#fi

	unzip $SATURN_TMP/sbl6.zip -d $SATURN_TMP

fi

exit 0
