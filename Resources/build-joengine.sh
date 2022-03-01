#!/bin/bash
set -e

if [ $INSTALL_JO_ENGINE_LIB -eq 1 ]; then

	if [ ! -d $SATURN_JOENGINE ]; then
		echo "$SATURN_JOENGINE does not exist !"
		exit 1
	fi

	#
	# copy Jo Engine
	#
  cp --verbose -rf $SATURN_TMP/jo/jo_engine $SATURN_JOENGINE
  cp --verbose -rf $SATURN_TMP/jo/Samples/ $SATURN_JOENGINE/
	mkdir -p $SATURN_JOENGINE/common/
  cp --verbose -rf $SATURN_TMP/jo/Compiler/COMMON/* $SATURN_JOENGINE/common/

	#
	# Patch the makefiles to point at the right places
	#
	find ./ -type f -name "makefile" \
			-exec sed -i 's/^JO_ENGINE_SRC_DIR.*$/JO_ENGINE_SRC_DIR=$(SATURN_JOENGINE)\/jo_engine/g' {} \;\
			-exec sed -i 's/^COMPILER_DIR.*$/COMPILER_DIR=$(INSTALLDIR)/g' {} \;\
			-exec sed -i 's/^include $(COMPILER_DIR)\/COMMON\/jo_engine_makefile.*$/include $(SATURN_JOENGINE)\/common\/jo_engine_makefile/g' {} \;

	#
	# Patch the main makefile
	#
	sed -i 's/CC=$(COMPILER_DIR)\/LINUX/CC=$(INSTALLDIR)/g' $SATURN_JOENGINE/common/jo_engine_makefile
	sed -i 's/CONV=$(COMPILER_DIR)\/LINUX/CONV=$(INSTALLDIR)/g' $SATURN_JOENGINE/common/jo_engine_makefile
	#sed -i 's/CONV=$(COMPILER_DIR)\/LINUX/CONV=$(INSTALLDIR)/g' $SATURN_JOENGINE/common/jo_engine_makefile

fi

exit 0
