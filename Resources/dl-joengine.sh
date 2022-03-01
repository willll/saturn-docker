#!/bin/bash
set -e

if [ $INSTALL_JO_ENGINE_LIB -eq 1 ]; then

  git clone --depth 1 -b master https://github.com/johannes-fetz/joengine.git "$SATURN_TMP/jo"

fi

exit 0
