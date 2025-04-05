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
	            --build-arg INSTALL_SATURNSDK_SAMPLES=0 \
              --build-arg INSTALL_JO_ENGINE_LIB=0 \
              --build-arg INSTALL_JO_ENGINE_SAMPLES=0 \
              --build-arg INSTALL_YAUL_LIB_ARG=0 \
              --build-arg INSTALL_YAUL_SAMPLES=0 \
              --build-arg INSTALL_SBL_LIB=0 \
              --build-arg INSTALL_SBL_SAMPLES=0 \
              --build-arg INSTALL_SBL_EXAMPLES=0 \
              --build-arg INSTALL_SGL_LIB=0 \
              --build-arg INSTALL_SGL_SAMPLES=0 \
              --build-arg INSTALL_SRL_LIB=0 \
              --build-arg INSTALL_IAPETUS_LIB=0 \
              --build-arg INSTALL_IAPETUS_SAMPLES=0 \
              --build-arg INSTALL_CYBERWARRIORX_CDC_LIB=0 \
	      -t willll/saturn-docker:gcc_$1_vanilla . --file ./Dockerfile || exit 1

# Publish to Dpcker hub
if [[ -n "$2" ]] && [[ "PUSH" == "$2" ]]; then
  docker push willll/saturn-docker:gcc_$1_vanilla || exit 1
fi

exit 0
