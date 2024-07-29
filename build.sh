#!/bin/bash
set -e

# Build the container
docker build --build-arg GCC_VERSION_ARG=$1 -t willll/saturn-docker:gcc_$1 . --file ./Dockerfile

# Publish to DOcker hub
docker push willll/saturn-docker:gcc_$1

exit 0
