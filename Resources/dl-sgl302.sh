#!/bin/bash
set -e

if [ ! -d $SATURN_TMP ]; then
	mkdir -p $SATURN_TMP
fi

cd $SATURN_TMP

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

#
# Extract
#
unzip $SATURN_TMP/sgl302.zip -d $SATURN_TMP

exit 0
