#!/bin/bash
set -e

if [ $INSTALL_YAUL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

	cd $SATURN_TMP

  git clone https://github.com/ijacquez/libyaul.git "$SATURN_TMP"

fi

exit 0
