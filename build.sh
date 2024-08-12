#!/bin/bash
set -e

# Build the container
docker build --no-cache \
              --build-arg MAKEFLAGS_ARG="-j `nproc`" \
              --build-arg GCC_VERSION_ARG=$1 \
              -t willll/saturn-docker:gcc_$1 . --file ./Dockerfile

# Publish to DOcker hub
docker push willll/saturn-docker:gcc_$1

exit 0
