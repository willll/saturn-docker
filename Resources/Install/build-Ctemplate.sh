#!/bin/bash
set -e

if [ ! -d $CTEMPLATE_ROOT ]; then
	mkdir -p $CTEMPLATE_ROOT
fi

#
# Install Ctemplate
#
git clone --depth 1 --branch $1 \
    https://github.com/OlafvdSpek/ctemplate.git "$SATURN_TMP/tmp"

cd "$SATURN_TMP/tmp/"

$SATURN_TMP/tmp/autogen.sh && $SATURN_TMP/tmp/configure --prefix=$CTEMPLATE_ROOT

make && make install

rm -rf "$SATURN_TMP/tmp"

exit 0
