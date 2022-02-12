#!/bin/bash
set -e

if [ $INSTALL_SAtURNSDK_SAMPLES -eq 1 ]; then

	git clone https://github.com/SaturnSDK/Saturn-SDK-Sample-00-Basic.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-00-Basic
	git clone https://github.com/SaturnSDK/Saturn-SDK-Sample-01-Sprite.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-01-Sprite
	git clone https://github.com/SaturnSDK/Saturn-SDK-Sample-02-TexturedSprite.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-02-TexturedSprite
	git clone https://github.com/SaturnSDK/Saturn-SDK-Sample-Common.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-Common
	git clone https://github.com/SaturnSDK/Saturn-SDK-Sample-13-CDBrowser.git \
	    "$SATURN_SAMPLES"/Saturn-SDK-Sample-13-CDBrowser

fi

exit 0
