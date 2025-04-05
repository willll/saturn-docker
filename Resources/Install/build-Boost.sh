#!/bin/bash
set -e

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
fi

if [ $INSTALL_BOOST_LIB -eq 1 ]; then

	if [ ! -d $BOOST_ROOT ]; then
		mkdir -p $BOOST_ROOT
	fi

	#
	# Install Boost preprocessor, System, filesystem, program_options
	# 	and dependancies ...
	#

	LIBS=(
						tools/build
						tools/boost_install
						libs/align
						libs/any
						libs/assert
						libs/array
						libs/atomic
						libs/bind
						libs/config
						libs/container_hash
						libs/concept_check
						libs/core
						libs/container
						libs/detail
						libs/exception
						libs/function
						libs/integer
						libs/intrusive
						libs/io
						libs/mpl
						libs/utility
						libs/move
						libs/numeric
						libs/lexical_cast
						libs/iterator
						libs/headers
						libs/predef
						libs/static_assert
						libs/range
						libs/smart_ptr
						libs/tokenizer
						libs/type_index
						libs/type_traits
						libs/throw_exception
						libs/preprocessor
						libs/filesystem
						libs/system
						libs/program_options
						)

	git clone --depth 1 --branch $1 \
		https://github.com/boostorg/boost.git "$SATURN_TMP/tmp"

	cd "$SATURN_TMP/tmp"

	for lib in "${LIBS[@]}"
	do
		git submodule update --init -- $lib
	done

	"$SATURN_TMP/tmp/bootstrap.sh" --prefix="$BOOST_ROOT"
	"$SATURN_TMP/tmp/b2" install
	"$SATURN_TMP/tmp/b2" headers

	rm -rf "$SATURN_TMP/tmp"

else
	echo "$(tput setaf 1)Boost is not installed$(tput sgr 0)"

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
