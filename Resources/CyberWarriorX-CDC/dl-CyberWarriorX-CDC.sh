#!/bin/bash
set -e

if [ $INSTALL_CYBERWARRIORX_CDC_LIB -eq 1 ]; then

  if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	wget -O $SATURN_TMP/cdcrepv100rc1.zip http://vberthelot.free.fr/SX/satdev/Tools/cyberWarriorX/cdcrepv100rc1.zip

	if [ ! -f "$SATURN_TMP/cdcrepv100rc1.zip" ]; then
	    echo "$SATURN_TMP/cdcrepv100rc1.zip not downloaded."
	    exit 1
	fi

	unzip $SATURN_TMP/cdcrepv100rc1.zip -d $SATURN_TMP/cdcrepv100rc1

fi

exit 0
