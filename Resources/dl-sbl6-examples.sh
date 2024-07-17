#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ $INSTALL_SBL_EXAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		echo "$SATURN_TMP does not exist !"
		exit 1
	fi

	gdown 19nKbK6eFHh1cliyFYsBv-P2IGrWTc0Ci -O "$SATURN_TMP/examples.zip"

	if [ ! -f "$SATURN_TMP/examples.zip" ]; then
	    echo "$SATURN_TMP/examples.zip not downloaded."
	    exit 1
	fi

	unzip $SATURN_TMP/examples.zip -d $SATURN_TMP

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
