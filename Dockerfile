# Sources :
# * https://github.com/misterkun-io/Toolchain_MiSTer
# * https://github.com/vbt1/Saturn-SDK-GCC-SH2
# * https://github.com/willll/Saturn-SDK-GCC-SH2
# * https://github.com/SaturnSDK/Saturn-SDK-GCC-SH2
# * https://github.com/shicky256/SaturnDev
# * https://github.com/johannes-fetz/joengine
# * https://github.com/SaturnSDK/Saturn-SDK-SGL
# * https://github.com/Pixinn/docker-vbcc-amiga

# * https://github.com/ijacquez/libyaul
# * https://github.com/ijacquez/libyaul-docker

FROM ubuntu:latest as linux
MAINTAINER willll "XXX@XXX.XXX"

SHELL ["/bin/bash", "-c"]

# Environment Variable
ENV DEBIAN_FRONTEND=noninteractive

env TERM=xterm

ENV SATURN_ROOT=/opt/saturn
ENV SATURN_SGL=$SATURN_ROOT/sgl
ENV SATURN_SBL=$SATURN_ROOT/sbl
ENV SATURN_CMAKE=$SATURN_ROOT/CMake
ENV SATURN_JOENGINE=$SATURN_ROOT/joengine
ENV SATURN_YAUL=$SATURN_ROOT/yaul
ENV SATURN_CD=$SATURN_ROOT/cd_resources
ENV SATURN_SAMPLES=$SATURN_ROOT/samples
ENV SATURN_IPMAKER=$SATURN_ROOT/IPMaker
ENV SATURN_COMMON=$SATURN_ROOT/common

ENV SATURN_TMP=$SATURN_ROOT/tmp
ENV PWD=$SATURN_TMP

ENV RELSRCDIR=./toolchain/source
ENV SRCDIR=$PWD/toolchain/source
ENV BUILDDIR=$PWD/toolchain/build
ENV INSTALLDIR=$SATURN_ROOT/toolchain
ENV SYSROOTDIR=$INSTALLDIR/sysroot
ENV ROOTDIR=$PWD/toolchain
ENV DOWNLOADDIR=$PWD/toolchain/download
ENV QTIFWDIR=./installer

ENV BOOST_ROOT=/opt/lib/boost
ENV BOOSTROOT=$BOOST_ROOT
ENV BOOST_INCLUDEDIR=$BOOST_ROOT/include
ENV BOOST_LIBRARYDIR=$BOOST_ROOT/lib
ENV Boost_NO_SYSTEM_PATHS="ON"

ENV CTEMPLATE_ROOT=/opt/lib/Ctemplate

ENV MAKEFLAGS="-j 1"

ENV GCC_VERSION=8.4.0

RUN apt-get update && apt-get install -y locales \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

# Set System Locales
RUN echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen && locale-gen
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# Core Development Packages
RUN apt-get update && apt-get install -y \
	build-essential \
	git \
	nano \
	unzip \
	wget \
	ca-certificates \
	dos2unix \
	gpg \
	bison \
	curl \
	texinfo \
	autotools-dev \
	automake \
  cmake \
  shtool \
  intltool \
  libtool \
  gettext \
  autogen \
	autoconf-archive \
  xutils-dev \
  xorriso \
	--no-install-recommends && \
	apt autoremove -y && \
	## Make sure we leave any X11 related library behind
	apt-get purge -y 'libx11*' x11-common libxt6 && \
	apt autoremove -y && \
	rm -r /var/lib/apt/lists/*

# Base Directories
RUN mkdir -p "${SATURN_ROOT}" "${SATURN_SGL}" "${SATURN_SBL}" \
  "${SATURN_CMAKE}" "${SATURN_JOENGINE}" "${SATURN_YAUL}" "${SATURN_TMP}" \
  "${SATURN_CD}" "${SATURN_SAMPLES}" "${SATURN_IPMAKER}" "${SATURN_COMMON}" && \
	chmod -R 777 "$SATURN_ROOT" && \
	chmod -R 777 "$SATURN_SGL" && \
	chmod -R 777 "$SATURN_SBL" && \
	chmod -R 777 "$SATURN_CMAKE" && \
	chmod -R 777 "$SATURN_JOENGINE" && \
	chmod -R 777 "$SATURN_YAUL" && \
  chmod -R 777 "$SATURN_CD" && \
  chmod -R 777 "$SATURN_SAMPLES" && \
  chmod -R 777 "$SATURN_IPMAKER" && \
  chmod -R 777 "$SATURN_COMMON" && \
  chmod -R 777 "$SATURN_TMP"

WORKDIR "${SATURN_ROOT}"

#
# Setup
#
COPY Resources/Install/setup-git.sh $SATURN_TMP
RUN $SATURN_TMP/setup-git.sh

#
# Install mkisofs
#
COPY Resources/Install/build-Mkisofs.sh $SATURN_TMP
RUN $SATURN_TMP/build-Mkisofs.sh

#
# Install Saturn-SDK-Tool-IPMaker 0.1
#
COPY Resources/Install/build-IPMaker.sh $SATURN_TMP
RUN $SATURN_TMP/build-IPMaker.sh "IPMaker_0.1"

#
# Install Boost preprocessor, System, filesystem, program_options 1.78.0
#
COPY Resources/Install/build-Boost.sh $SATURN_TMP
RUN $SATURN_TMP/build-Boost.sh "boost-1.78.0"

#
# Install CueMaker 1.0
#
COPY Resources/Install/build-CueMaker.sh $SATURN_TMP
RUN $SATURN_TMP/build-CueMaker.sh "CueMaker_1.0"

# Clean up
RUN rm -rf "$SATURN_TMP"

#
# Install base tools
#

ENV PROGRAM_PREFIX=sh-elf-
ENV NCPU=1
ENV CREATEINSTALLER="NO"

ENV TARGETMACH=sh-elf
ENV BUILDMACH=i686-pc-linux-gnu
ENV HOSTMACH=i686-pc-linux-gnu
ENV OBJFORMAT=ELF

ENV BINUTILS_CFLAGS="-s"
ENV GCC_BOOTSTRAP_FLAGS="--with-cpu=m2"
ENV GCC_FINAL_FLAGS="--with-cpu=m2 --with-sysroot=$SYSROOTDIR"

RUN git clone --depth 1 --branch gcc_$GCC_VERSION \
    https://github.com/willll/Saturn-SDK-GCC-SH2.git "$SATURN_TMP"

WORKDIR "${SATURN_TMP}"

RUN chmod 777 *.sh

# Clean up Windows's mess
RUN dos2unix *

RUN chmod +x *.sh

RUN . ./versions.sh && \
      ./build-elf.sh

RUN rm -rf "$SATURN_TMP"

# Set GCC env variables and flags
ENV CXXFLAGS=""
ENV LD_LIBRARY_PATH="${INSTALLDIR}/lib:${LD_LIBRARY_PATH}"
ENV CPATH="${INSTALLDIR}/include:${CPATH}"
ENV COMPILER_PATH="${INSTALLDIR}/include:${COMPILER_PATH}"
ENV C_INCLUDE_PATH="${INSTALLDIR}/include:${C_INCLUDE_PATH}"
ENV CPLUS_INCLUDE_PATH="${INSTALLDIR}/include:${CPLUS_INCLUDE_PATH}"
ENV CC=${INSTALLDIR}/bin/${PROGRAM_PREFIX}gcc
ENV CXX=${INSTALLDIR}/bin/${PROGRAM_PREFIX}g++
ENV AR=${INSTALLDIR}/bin/${PROGRAM_PREFIX}ar
ENV AS=${INSTALLDIR}/bin/${PROGRAM_PREFIX}as
ENV LD=${INSTALLDIR}/bin/${PROGRAM_PREFIX}ld
ENV RANLIB=${INSTALLDIR}/bin/${PROGRAM_PREFIX}ranlib
ENV OBJCOPY=${INSTALLDIR}/bin/${PROGRAM_PREFIX}objcopy

# Set PATH to access compilers
ENV PATH="${INSTALLDIR}/bin:${PATH}"

# Copy CMake configuration files
COPY Resources/CMake/* $SATURN_CMAKE/

# Copy common files for all the projects
COPY Resources/Common/* $SATURN_COMMON/

# Install Files for ISO creation
COPY Resources/CD $SATURN_CD

#
# LIBRARIES SETUP
#

ENV INSTALL_SGL_LIB=1
ENV INSTALL_SGL_SAMPLES=0

ENV INSTALL_SBL_LIB=1
ENV INSTALL_SBL_SAMPLES=1
ENV INSTALL_SBL_EXAMPLES=0

ENV INSTALL_SATURNSDK_SAMPLES=0

ENV INSTALL_JO_ENGINE_LIB=1
ENV INSTALL_JO_ENGINE_SAMPLES=0

ENV INSTALL_YAUL_LIB=1
ENV INSTALL_YAUL_SAMPLES=0


#
# Install SGL
#

ENV SEGASGL=${SATURN_SGL}

WORKDIR "${SATURN_TMP}"

COPY Resources/dl-sgl302.sh $SATURN_TMP
RUN $SATURN_TMP/dl-sgl302.sh
COPY Resources/build-sgl302.sh $SATURN_TMP
RUN $SATURN_TMP/build-sgl302.sh $SATURN_SGL

#
# Install SBL
#

# Download SBL
ENV SEGALIB=${SATURN_SBL}/segalib
ENV SEGASMP=${SATURN_SBL}/segasmp

COPY Resources/dl-sbl6.sh $SATURN_TMP
RUN $SATURN_TMP/dl-sbl6.sh

COPY Resources/sbl6/segalib $SATURN_TMP/sbl6_/segalib
COPY Resources/sbl6/segasmp $SATURN_TMP/sbl6_/segasmp
COPY Resources/build-sbl6-lib.sh $SATURN_TMP
COPY Resources/build-sbl6-samples.sh $SATURN_TMP
COPY Resources/sbl6.patch $SATURN_TMP
RUN $SATURN_TMP/build-sbl6-lib.sh

RUN rm -rf "$SATURN_TMP/*"

# Download SBL examples
COPY Resources/dl-sbl6-examples.sh $SATURN_TMP
RUN $SATURN_TMP/dl-sbl6-examples.sh
COPY Resources/sbl6/examples $SATURN_TMP/sbl6_/examples
COPY Resources/build-sbl6-examples.sh $SATURN_TMP
COPY Resources/sbl6_examples.patch $SATURN_TMP
RUN $SATURN_TMP/build-sbl6-examples.sh

RUN rm -rf "$SATURN_TMP/*"

#
# Samples
#

COPY Resources/dl-SaturnSDK-samples.sh $SATURN_SAMPLES
COPY Resources/build-SaturnSDK-samples.sh $SATURN_SAMPLES
RUN $SATURN_SAMPLES/dl-SaturnSDK-samples.sh
COPY Resources/Samples $SATURN_SAMPLES
RUN $SATURN_SAMPLES/build-SaturnSDK-samples.sh
RUN rm -rf "$SATURN_TMP/*"


#
# Install Jo Engine
#

COPY Resources/dl-joengine.sh "$SATURN_TMP"
RUN "$SATURN_TMP/dl-joengine.sh"
COPY Resources/build-joengine.sh "$SATURN_TMP"
COPY Resources/jo-engine/jo_engine_makefile "$SATURN_TMP"
RUN "$SATURN_TMP/build-joengine.sh"
COPY Resources/build-joengine-samples.sh "$SATURN_TMP"
RUN "$SATURN_TMP/build-joengine-samples.sh"

RUN rm -rf "$SATURN_TMP/*"

#
# Install Yaul
#

COPY Resources/dl-yaul.sh "$SATURN_TMP"
RUN "$SATURN_TMP/dl-yaul.sh"
COPY Resources/yaul/.yaul.env "$SATURN_YAUL"
COPY Resources/yaul/env.mk "$SATURN_TMP/yaul"
COPY Resources/yaul/pre.common.mk "$SATURN_TMP/yaul/libyaul/common/"
COPY Resources/yaul/Makefile "$SATURN_TMP/yaul"
COPY Resources/yaul/tools/bin2o "$SATURN_TMP/yaul/tools/bin2o/"
COPY Resources/yaul/tools/make-ip "$SATURN_TMP/yaul/tools/make-ip/"
COPY Resources/yaul/tools/Makefile "$SATURN_TMP/yaul/tools/Makefile"
COPY Resources/yaul/common/specs/* "$SATURN_TMP/yaul/libyaul/common/specs/"
COPY Resources/yaul/common/specs/* "$SATURN_TMP/yaul/libyaul/common/specs/"
COPY Resources/yaul/common/ldscripts/yaul.x "$SATURN_TMP/yaul/libyaul/common/ldscripts/"
COPY Resources/build-yaul.sh "$SATURN_TMP"
RUN "$SATURN_COMMON/set_env.sh" "$SATURN_TMP/build-yaul.sh"
COPY Resources/build-yaul-examples.sh $SATURN_TMP
RUN "$SATURN_COMMON/set_env.sh" "$SATURN_TMP/build-yaul-examples.sh"

# TODO : Add https://github.com/ijacquez/saturn-compos.git

# Clean up temporary files
RUN rm -rf "$SATURN_TMP/*"

# Set Volume and Workdir
VOLUME /saturn
WORKDIR /opt/saturn/tmp/

# Bash Settings
RUN echo "export HISTTIMEFORMAT='%d/%m/%y %T '" >> ~/.bashrc && \
    echo "export PS1='\[\e[0;36m\]\u\[\e[0m\]@\[\e[0;33m\]\h\[\e[0m\]:\[\e[0;35m\]\w\[\e[0m\]\$ '" >> ~/.bashrc && \
    echo "alias ll='ls -lah'" >> ~/.bashrc && \
    echo "alias ls='ls --color=auto'" >> ~/.bashrc


# Metadata Params
ARG BUILD_DATE
ARG VCS_REF
ARG BUILD_VERSION

# Metadata
LABEL \
	org.label-schema.schema-version="1.0" \
	org.label-schema.vendor="willll" \
	org.label-schema.name="willll/saturn-docker" \
	org.label-schema.description="SH2 SuperH Compiler" \
	org.label-schema.url="https://github.com/willll/saturn-docker" \
	org.label-schema.vcs-url="https://github.com/willll/saturn-docker.git" \
	org.label-schema.build-date=$BUILD_DATE \
	org.label-schema.vcs-ref=$VCS_REF \
	org.label-schema.version=$BUILD_VERSION \
	org.label-schema.docker.cmd="docker run -it --rm -v ${pwd}:/saturn saturn-docker"

ENTRYPOINT ["/opt/saturn/common/set_env.sh"]
CMD ["/bin/bash"]
