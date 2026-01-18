#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

if [ $INSTALL_JO_ENGINE_LIB -eq 1 ]; then

  git clone -b $JO_ENGINE_TAG --depth 1 https://github.com/johannes-fetz/joengine.git "$SATURN_TMP/jo"

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
