#!/bin/bash
set -euo pipefail

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

: "${INSTALL_BOOST_LIB:?INSTALL_BOOST_LIB is not set}"
: "${BOOST_ROOT:?BOOST_ROOT is not set}"
: "${SATURN_TMP:?SATURN_TMP is not set}"

if ! command -v git >/dev/null 2>&1; then
	echo "git is required but not installed" >&2
	exit 1
fi

if [ "$INSTALL_BOOST_LIB" -eq 1 ]; then
	if [ "$#" -lt 1 ] || [ -z "${1:-}" ]; then
		echo "Boost git branch is required as the first argument" >&2
		exit 1
	fi

	if [ ! -d "$BOOST_ROOT" ]; then
		mkdir -p "$BOOST_ROOT"
	fi

	#
	# Install Boost preprocessor, System, filesystem, program_options
	# 	and dependancies ...
	#

	LIBS=(
						tools/build
						tools/boost_install
						libs/algorithm
						libs/align
						libs/any
						libs/array
						libs/asio
						libs/assert
						libs/atomic
						libs/beast
						libs/bind
						libs/chrono
						libs/concept_check
						libs/config
						libs/container
						libs/container_hash
						libs/core
						libs/date_time
						libs/describe
						libs/detail
						libs/endian
						libs/exception
						libs/filesystem
						libs/foreach
						libs/function
						libs/headers
						libs/integer
						libs/intrusive
						libs/io
						libs/iterator
						libs/lexical_cast
						libs/logic
						libs/move
						libs/mp11
						libs/mpl
						libs/numeric
						libs/optional
						libs/predef
						libs/preprocessor
						libs/program_options
						libs/property_tree
						libs/multi_index
						libs/serialization
						libs/range
						libs/smart_ptr
						libs/static_assert
						libs/system
						libs/throw_exception
						libs/tokenizer
						libs/tuple
						libs/type_index
						libs/type_traits
						libs/utility
						libs/variant
						libs/winapi
						)

	git clone --depth 1 --branch "$1" \
		https://github.com/boostorg/boost.git "$SATURN_TMP/tmp"

	cd "$SATURN_TMP/tmp"

	for lib in "${LIBS[@]}"
	do
		git submodule update --init --recursive -- $lib
	done

	"$SATURN_TMP/tmp/bootstrap.sh" --prefix="$BOOST_ROOT"
	"$SATURN_TMP/tmp/b2" install --with-filesystem --with-program_options --with-system
	"$SATURN_TMP/tmp/b2" headers

	rm -rf "$SATURN_TMP/tmp"

else
	echo "$(tput setaf 1)Boost is not installed$(tput sgr 0)"

fi

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
