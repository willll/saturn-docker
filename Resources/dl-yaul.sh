#!/bin/bash
set -e

if [ $INSTALL_YAUL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

  git clone --depth 1 https://github.com/ijacquez/libyaul.git "$SATURN_TMP/yaul"

fi

exit 0
