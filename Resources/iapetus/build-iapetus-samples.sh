#!/bin/bash
set -e


if [ $INSTALL_IAPETUS_SAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_IAPETUS ]; then
		echo "$SATURN_IAPETUS does not exist !"
		exit 1
	fi

else
	echo "$(tput setaf 1)No iapetus samples will be built$(tput sgr 0)"

fi

exit 0
