#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

git config --global advice.detachedHead false

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
