#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ $INSTALL_SATURNSDK_SAMPLES -eq 1 ]; then

	git clone --depth 1 https://github.com/SaturnSDK/Saturn-SDK-Sample-00-Basic.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-00-Basic
	git clone --depth 1 https://github.com/SaturnSDK/Saturn-SDK-Sample-01-Sprite.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-01-Sprite
	git clone --depth 1 https://github.com/SaturnSDK/Saturn-SDK-Sample-02-TexturedSprite.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-02-TexturedSprite
	git clone --depth 1 https://github.com/SaturnSDK/Saturn-SDK-Sample-Common.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-Common
	git clone --depth 1 https://github.com/SaturnSDK/Saturn-SDK-Sample-13-CDBrowser.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-13-CDBrowser

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
