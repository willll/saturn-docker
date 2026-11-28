#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

if [ $INSTALL_SBL63_LIB -eq 1 ]; then

	cd "$SATURN_SBL63"

	# Libraries go to segalib/lib_elf and sgl/lib_elf, samples to elf/ in their own folder
	PREFIX=$PROGRAM_PREFIX bash build.sh libs

	if [ $INSTALL_SBL63_SAMPLES -eq 1 ]; then
		PREFIX=$PROGRAM_PREFIX bash build.sh samples
	else
		echo "$(tput setaf 1)No SBL 6.3 samples will be built$(tput sgr 0)"
	fi

else
	echo "$(tput setaf 1)No SBL 6.3 will be installed$(tput sgr 0)"

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
