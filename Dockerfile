# Sources :
# * https://github.com/misterkun-io/Toolchain_MiSTer

# * https://github.com/vbt1/Saturn-SDK-GCC-SH2
# * https://github.com/willll/Saturn-SDK-GCC-SH2
# * https://github.com/SaturnSDK/Saturn-SDK-GCC-SH2

# * https://github.com/willll/Saturn-SDK-GCC-M68K

# * https://github.com/shicky256/SaturnDev

# * https://github.com/johannes-fetz/joengine

# * https://github.com/SaturnSDK/Saturn-SDK-SGL

# * https://github.com/Pixinn/docker-vbcc-amiga

# * https://github.com/yaul-org/libyaul
# * https://github.com/yaul-org/libyaul-docker
# * https://github.com/yaul-org/libyaul-examples

# * https://github.com/ReyeMe/SaturnRingLib

# * https://github.com/cyberwarriorx/iapetus

# * http://vberthelot.free.fr/SX/satdev/Tools.html

FROM ubuntu:noble AS linux

SHELL ["/bin/bash", "-c"]

# Environment Variable
ENV DEBIAN_FRONTEND=noninteractive

ENV TERM=xterm

ENV SATURN_ROOT=/opt/saturn

ENV SATURN_SGL=$SATURN_ROOT/sgl
ENV SATURN_SBL=$SATURN_ROOT/sbl
ENV SATURN_CMAKE=$SATURN_ROOT/CMake
ENV SATURN_JOENGINE=$SATURN_ROOT/joengine
ENV SATURN_YAUL=$SATURN_ROOT/yaul
ENV SATURN_SRL=$SATURN_ROOT/SaturnRingLib           
ENV SATURN_CYBERWARRIORX=$SATURN_ROOT/cyberwarriorx
ENV SATURN_IAPETUS=$SATURN_CYBERWARRIORX/iapetus
ENV SATURN_CYBERWARRIORX_CDC=$SATURN_CYBERWARRIORX/cdc
ENV SATURN_CD=$SATURN_ROOT/cd_resources
ENV SATURN_SAMPLES=$SATURN_ROOT/samples
ENV SATURN_IPMAKER=$SATURN_ROOT/IPMaker
ENV SATURN_SATCONV=$SATURN_ROOT/satconv
ENV SATURN_COMMON=$SATURN_ROOT/common

ENV SATURN_TMP=$SATURN_ROOT/tmp
ENV PWD=$SATURN_TMP

ENV RELSRCDIR=./toolchain/source
ENV QTIFWDIR=./installer

ENV BOOST_ROOT=/opt/lib/boost
ENV BOOSTROOT=$BOOST_ROOT
ENV BOOST_INCLUDEDIR=$BOOST_ROOT/include
ENV BOOST_LIBRARYDIR=$BOOST_ROOT/lib
ENV Boost_NO_SYSTEM_PATHS="ON"

ENV CTEMPLATE_ROOT=/opt/lib/Ctemplate

ARG MAKEFLAGS_ARG="-j 1"
ENV MAKEFLAGS=$MAKEFLAGS_ARG

RUN apt-get update && apt-get install -y locales \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

# Set System Locales
RUN echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen && locale-gen
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# COPY Resources/Install/ubuntu-packages.list /tmp

# Core Development Packages
RUN apt-get update \
  && apt-get install --no-install-recommends -y \
  build-essential git nano unzip wget \
  ca-certificates dos2unix gpg bison \
  curl texinfo autotools-dev automake \
  cmake shtool intltool libtool gettext \
  autogen autoconf-archive xutils-dev \
  xorriso doxygen ffmpeg ninja-build \
  python3-pip python-is-python3 openssh-server \
  rsync zip pipx libgmp-dev libmpfr-dev sox \
  libsox-fmt-all libsox-fmt-mp3 \
  ## Make sure we leave any X11 related library behind
  && apt-get purge -y 'libx11*' x11-common libxt6 \
  && apt autoremove -y --purge \
  && rm -r /var/lib/apt/lists/*

# Base Directories
RUN for directory in ${SATURN_ROOT} ${SATURN_SGL} ${SATURN_SBL} \
                      ${SATURN_CMAKE} ${SATURN_JOENGINE} ${SATURN_YAUL} \
                      ${SATURN_IAPETUS} ${SATURN_TMP} "${SATURN_CD}" "${SATURN_SAMPLES}" \
                      "${SATURN_IPMAKER}" "${SATURN_SATCONV}" "${SATURN_COMMON}" \
                      "${SATURN_CYBERWARRIORX_CDC}" "${SATURN_SRL}";\
    do mkdir -p "$directory" && chmod -R 777 "$directory"; done

WORKDIR "${SATURN_ROOT}"

# pipx setup
RUN pipx ensurepath && pipx completions

FROM linux AS tools

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
# Install Saturn-SDK-Tool-IPMaker 0.2
#
COPY Resources/Install/build-IPMaker.sh $SATURN_TMP
RUN $SATURN_TMP/build-IPMaker.sh "IPMaker_0.2"

#
# Install Boost preprocessor, System, filesystem, program_options 1.78.0
#
ARG INSTALL_BOOST_LIB=1
COPY Resources/Install/build-Boost.sh $SATURN_TMP
RUN $SATURN_TMP/build-Boost.sh "boost-1.78.0"

#
# Install CueMaker 1.0
#
COPY Resources/Install/build-CueMaker.sh $SATURN_TMP
RUN $SATURN_TMP/build-CueMaker.sh "CueMaker_1.0"

#
# Install satconv
#
COPY Resources/Install/build-satconv.sh $SATURN_TMP
RUN $SATURN_TMP/build-satconv.sh
ENV PATH="$PATH:$SATURN_SATCONV"

#
# Install gdown https://pypi.org/project/gdown/
#
RUN pipx install gdown
RUN echo -e '#!/bin/bash\npipx run gdown "$@"' > /usr/bin/gdown \
    && chmod +x /usr/bin/gdown

# Clean up
RUN rm -rf "$SATURN_TMP"

#
# Install base tools
#

# GCC_VERSION_ARG : must match tags from https://github.com/willll/Saturn-SDK-GCC-SH2/tags
ARG GCC_VERSION_ARG=14.2.0_0
ENV GCC_VERSION=$GCC_VERSION_ARG

ARG NCPU=1
ARG CREATEINSTALLER="NO"

ENV BUILDMACH=i686-pc-linux-gnu
ENV HOSTMACH=i686-pc-linux-gnu

COPY Resources/Toolchain/* $SATURN_TMP/

WORKDIR "${SATURN_TMP}"

FROM tools AS m68k

#
# Motorola 68EC000 (SCSP) compiler
#

ARG INSTALL_M68K_GCC=0

ENV ROOTDIR=$PWD/m68k_toolchain
ENV SRCDIR=$ROOTDIR/source
ENV BUILDDIR=$ROOTDIR/build
ENV INSTALLDIR=$SATURN_ROOT/m68k_toolchain
ENV SYSROOTDIR=$INSTALLDIR/sysroot
ENV DOWNLOADDIR=$ROOTDIR/download

ENV BUILD_FOLDER="${SATURN_TMP}/m68k"

ENV BINUTILS_CFLAGS=""
ENV GCC_BOOTSTRAP_FLAGS=""
ENV GCC_FINAL_FLAGS=""

RUN ./dl-m68k.sh
RUN ./build-m68k-elf.sh

FROM m68k AS sh2

#
# Hitashi SH2 compiler
#

ENV SRCDIR=$PWD/toolchain/source
ENV BUILDDIR=$PWD/toolchain/build
ENV INSTALLDIR=$SATURN_ROOT/toolchain
ENV SYSROOTDIR=$INSTALLDIR/sysroot
ENV ROOTDIR=$PWD/toolchain
ENV DOWNLOADDIR=$PWD/toolchain/download

ENV BINUTILS_CFLAGS="-s"
ENV GCC_BOOTSTRAP_FLAGS="--with-cpu=m2"
ENV GCC_FINAL_FLAGS="--with-cpu=m2 --with-sysroot=$SYSROOTDIR"

ENV BUILD_FOLDER="${SATURN_TMP}/elf"

RUN ./dl-SDK.sh
RUN ./build-SDK-elf.sh

RUN rm -rf "$SATURN_TMP"

# Set GCC env variables and flags
ENV PROGRAM_PREFIX=sh-elf-
ENV TARGETMACH=sh-elf
ENV OBJFORMAT=ELF
ENV CXXFLAGS=""
ENV LD_LIBRARY_PATH="${INSTALLDIR}/lib:${INSTALLDIR}/sh-elf/lib:${LD_LIBRARY_PATH}"
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

# CMAKE variables
ENV CMAKE_MODULE_PATH=${SATURN_CMAKE}

# Set GCC for SH2 the default
RUN source $SATURN_COMMON/gcc_sh2.env

# Add GCC SH2 to alternatives
# RUN update-alternatives --install /usr/bin/gcc gcc "${CC}" 100 \
#                        --slave /usr/bin/g++ g++ "${CXX}"

#
# LIBRARIES SETUP
#

ARG BUILD_TYPE_ARG=Release
ENV BUILD_TYPE=$BUILD_TYPE_ARG

FROM sh2 AS sgl

#
# Install SGL
#

ARG INSTALL_SGL_LIB=1
ARG INSTALL_SGL_SAMPLES=1

ENV SEGASGL=${SATURN_SGL}

WORKDIR "${SATURN_TMP}"

COPY Resources/dl-sgl302.sh $SATURN_TMP
RUN $SATURN_TMP/dl-sgl302.sh
COPY Resources/build-sgl302.sh $SATURN_TMP
COPY Resources/sgl302.patch $SATURN_TMP
RUN $SATURN_TMP/build-sgl302.sh $SATURN_SGL

FROM sgl AS sbl

#
# Install SBL
#

ARG INSTALL_SBL_LIB=1
ARG INSTALL_SBL_SAMPLES=1
ARG INSTALL_SBL_EXAMPLES=1

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
RUN $SATURN_TMP/build-sbl6-samples.sh

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
# Install SGL samples
#

COPY Resources/sgl $SATURN_TMP/sgl_
COPY Resources/build-sgl302-samples.sh $SATURN_TMP
RUN $SATURN_TMP/build-sgl302-samples.sh $SATURN_SGL

FROM sbl AS sbl-samples

#
# Samples
#

ARG INSTALL_SATURNSDK_SAMPLES=1

COPY Resources/dl-SaturnSDK-samples.sh $SATURN_SAMPLES
COPY Resources/build-SaturnSDK-samples.sh $SATURN_SAMPLES
RUN $SATURN_SAMPLES/dl-SaturnSDK-samples.sh
COPY Resources/Samples $SATURN_SAMPLES
RUN $SATURN_SAMPLES/build-SaturnSDK-samples.sh
RUN rm -rf "$SATURN_TMP/*"

FROM sbl-samples AS joengine

#
# Install Jo Engine
#

ARG INSTALL_JO_ENGINE_LIB=1
ARG INSTALL_JO_ENGINE_SAMPLES=1
ARG JO_ENGINE_TAG=v2025.1

ENV JO_COMPILE_USING_SGL=1
ENV JO_COMPILE_WITH_FS_MODULE=1
#ENV JO_COMPILE_WITH_CONSOLE_SUPPORT=1
ENV JO_COMPILE_WITH_FAST_BUT_LESS_ACCURATE_MATH=1
ENV JO_COMPILE_WITH_VIDEO_MODULE=1
ENV JO_COMPILE_WITH_BACKUP_MODULE=1
ENV JO_COMPILE_WITH_SOFTWARE_RENDERER_MODULE=1
ENV JO_COMPILE_WITH_AUDIO_MODULE=1
ENV JO_COMPILE_WITH_3D_MODULE=1
ENV JO_COMPILE_WITH_STORYBOARD_MODULE=1
ENV JO_COMPILE_WITH_PSEUDO_MODE7_MODULE=1
ENV JO_COMPILE_WITH_EFFECTS_MODULE=1
ENV JO_COMPILE_WITH_PRINTF_MODULE=1
ENV JO_COMPILE_WITH_SPRITE_HASHTABLE=1
ENV JO_COMPILE_WITH_RAM_CARD_MODULE=1
ENV JO_COMPILE_WITH_DUAL_CPU_MODULE=1

COPY Resources/dl-joengine.sh "$SATURN_TMP"
RUN "$SATURN_TMP/dl-joengine.sh"
COPY Resources/build-joengine.sh "$SATURN_TMP"
COPY Resources/jo-engine/jo_engine_makefile "$SATURN_TMP"
RUN "$SATURN_TMP/build-joengine.sh"
COPY Resources/build-joengine-samples.sh "$SATURN_TMP"
RUN "$SATURN_TMP/build-joengine-samples.sh"

RUN rm -rf "$SATURN_TMP/*"

FROM joengine AS yaul

#
# Install Yaul
#

ARG INSTALL_YAUL_LIB_ARG=1
ENV INSTALL_YAUL_LIB=$INSTALL_YAUL_LIB_ARG
ARG INSTALL_YAUL_SAMPLES=0
ARG YAUL_TAG=0.3.1
ARG YAUL_EXAMPLES_COMMIT_SHA=21fd76d83dffd49afc4926d6a8408eecfec474f5

COPY Resources/dl-yaul.sh "$SATURN_TMP"
RUN "$SATURN_TMP/dl-yaul.sh"
COPY Resources/yaul/ "$SATURN_TMP/yaul/"
COPY Resources/build-yaul.sh "$SATURN_TMP"
COPY Resources/yaul/yaul.env.in "$SATURN_YAUL"
RUN "$SATURN_TMP/yaul/set_env.sh" "$SATURN_TMP/build-yaul.sh"
COPY Resources/build-yaul-examples.sh $SATURN_TMP
RUN "$SATURN_TMP/yaul/set_env.sh" "$SATURN_TMP/build-yaul-examples.sh"

# TODO : Add https://github.com/ijacquez/saturn-compos.git

# Clean up temporary files
RUN rm -rf "$SATURN_TMP/*"

FROM yaul AS iapetus

#
# Install Iapetus
#

ARG INSTALL_IAPETUS_SAMPLES=0
ARG INSTALL_IAPETUS_LIB=0
  # IAPETUS commit from 2019.03.19 https://github.com/cyberwarriorx/iapetus/tree/955d7c50f634cdd18722657c920987200d9ba3a5
ARG IAPETUS_COMMIT_SHA=955d7c50f634cdd18722657c920987200d9ba3a5

COPY Resources/iapetus/dl-iapetus.sh "$SATURN_TMP"
RUN "$SATURN_TMP/dl-iapetus.sh"
COPY Resources/iapetus/build-iapetus.sh "$SATURN_TMP"
RUN "$SATURN_TMP/build-iapetus.sh"
COPY Resources/iapetus/build-iapetus-samples.sh "$SATURN_TMP"
RUN "$SATURN_TMP/build-iapetus-samples.sh"

RUN rm -rf "$SATURN_TMP/*"

FROM iapetus AS cdc

#
# CyberWarriorX's CDC Library : Reverse-engineering of the Sega's CDC library
#

ARG INSTALL_CYBERWARRIORX_CDC_LIB=0

COPY Resources/CyberWarriorX-CDC/dl-CyberWarriorX-CDC.sh "$SATURN_TMP"
RUN "$SATURN_TMP/dl-CyberWarriorX-CDC.sh"
COPY Resources/CyberWarriorX-CDC/CMakeLists.txt "$SATURN_CYBERWARRIORX_CDC"
COPY Resources/CyberWarriorX-CDC/build-CyberWarriorX-CDC.sh "$SATURN_TMP"
RUN "$SATURN_TMP/build-CyberWarriorX-CDC.sh" $SATURN_CYBERWARRIORX_CDC

FROM cdc AS srl

#
# SRL : Easy to use SGL wrapper written in C++
#

ARG INSTALL_SRL_LIB=1
ARG SRL_LIB_TAG=0.9

COPY Resources/SRL/dl-SRL.sh "$SATURN_TMP"
RUN "$SATURN_TMP/dl-SRL.sh"
COPY Resources/SRL/build-SRL.sh "$SATURN_TMP"
RUN "$SATURN_TMP/build-SRL.sh"

FROM srl AS end

# Set Volume and Workdir
VOLUME /saturn
WORKDIR /opt/saturn/tmp/

# Bash Settings
RUN echo "export HISTTIMEFORMAT='%d/%m/%y %T '" >> ~/.bashrc \
    && echo "export PS1='\[\e[0;36m\]\u\[\e[0m\]@\[\e[0;33m\]\h\[\e[0m\]:\[\e[0;35m\]\w\[\e[0m\]\$ '" >> ~/.bashrc \
    && echo "alias ll='ls -lah'" >> ~/.bashrc \
    && echo "alias ls='ls --color=auto'" >> ~/.bashrc

# Establish the operating directory of OpenSSH
RUN mkdir -p /var/run/sshd

# Set Root password
RUN echo 'root:root' | chpasswd

# For remote connection (VS Code)
RUN echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config \
    && echo 'PermitEmptyPasswords yes' >> /etc/ssh/sshd_config \
    && echo 'PasswordAuthentication yes' >> /etc/ssh/sshd_config \
    && echo 'Port 22' >> /etc/ssh/sshd_config \
    && ssh-keygen -A \
    && echo -e 'if [[ -n $SSH_CONNECTION ]] ; then\n /opt/saturn/common/startup.sh \n fi\n' >> ~/.bashrc

# SSH login fix
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional \
    pam_loginuid.so@g' -i /etc/pam.d/sshd

# Authorize SSH Host
RUN mkdir -p /root/.ssh && \
    chmod 0700 /root/.ssh

# Add the keys and set permissions
ARG ssh_pub_key
RUN echo "$ssh_pub_key" > /root/.ssh/id_rsa.pub && \
    chmod 600 /root/.ssh/id_rsa.pub && \
    cat /root/.ssh/id_rsa.pub >> /root//.ssh/authorized_keys && \
    chmod 700 /root/.ssh && chmod 600 /root/.ssh/authorized_keys

# Freeze environment variables for SSH login
RUN "$SATURN_COMMON/set_env.sh"

# Metadata Params
ARG BUILD_DATE
ARG VCS_REF
ARG BUILD_VERSION

# Metadata
LABEL \
	org.label-schema.schema-version="1.0" \
	org.label-schema.vendor="willll" \
  org.opencontainers.image.authors="willll" \
	org.label-schema.name="willll/saturn-docker" \
	org.label-schema.description="SH2 SuperH Compiler" \
	org.label-schema.url="https://github.com/willll/saturn-docker" \
	org.label-schema.vcs-url="https://github.com/willll/saturn-docker.git" \
	org.label-schema.build-date=$BUILD_DATE \
	org.label-schema.vcs-ref=$VCS_REF \
	org.label-schema.version=$BUILD_VERSION \
	org.label-schema.docker.cmd="docker run -it -p 2222:22 --rm -v ${pwd}:/saturn saturn-docker"

ENTRYPOINT ["/opt/saturn/common/startup.sh"]

EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]
