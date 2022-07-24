#!/bin/bash
set -e

if [ $INSTALL_IAPETUS_LIB -eq 1 ]; then

  git clone -n https://github.com/cyberwarriorx/iapetus.git "$SATURN_TMP/iapetus"
  pushd "$SATURN_TMP/iapetus"
  git checkout $IAPETUS_COMMIT_SHA
  popd

fi

exit 0
