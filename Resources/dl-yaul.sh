#!/bin/bash
set -e

if [ $INSTALL_YAUL_LIB -eq 1 ]; then

	if [ ! -d $SATURN_TMP ]; then
		mkdir -p $SATURN_TMP
	fi

  git clone -n https://github.com/ijacquez/libyaul.git "$SATURN_TMP/yaul"
  pushd "$SATURN_TMP/yaul"
  git checkout $YAUL_COMMIT_SHA
  popd

else

	# create temp directories to avoid Docker errors
	mkdir -p "$SATURN_TMP/yaul/libyaul/common/ldscripts/"
	mkdir -p "$SATURN_TMP/yaul/libyaul/common/specs/"
	mkdir -p "$SATURN_TMP/yaul/tools/bin2o/"
	mkdir -p "$SATURN_TMP/yaul/tools/make-ip/"

fi

exit 0
