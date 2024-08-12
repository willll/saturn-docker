#!/bin/bash
set -e

if [[ $# -eq 0 ]] ; then
    echo 'Please provide a version of GCC to build'
    exit 1
fi

# Build the container
docker build --no-cache \
              --build-arg MAKEFLAGS_ARG="-j `nproc`" \
              --build-arg GCC_VERSION_ARG=$1 \
              -t willll/saturn-docker:gcc_$1 . --file ./Dockerfile || exit 1

# Publish to Dpcker hub
if [[ -n "$2" ]] && [[ "PUSH" == "$2" ]]; then
  docker push willll/saturn-docker:gcc_$1 || exit 1
fi

exit 0
