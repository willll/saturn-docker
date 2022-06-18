#!/bin/bash
set -e

if [ $INSTALL_JO_ENGINE_LIB -eq 1 ]; then

  git clone -n https://github.com/johannes-fetz/joengine.git "$SATURN_TMP/jo"
  pushd "$SATURN_TMP/jo"
  git checkout $JO_ENGINE_COMMIT_SHA
  popd

fi

exit 0
