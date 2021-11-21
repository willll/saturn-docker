# Sources :
# * https://github.com/misterkun-io/Toolchain_MiSTer
# * https://github.com/vbt1/Saturn-SDK-GCC-SH2
# * https://github.com/willll/Saturn-SDK-GCC-SH2
# * https://github.com/SaturnSDK/Saturn-SDK-GCC-SH2
# * https://github.com/shicky256/SaturnDev
# * https://github.com/johannes-fetz/joengine
# * https://github.com/SaturnSDK/Saturn-SDK-SGL

FROM ubuntu:latest as linux
MAINTAINER willll "XXX@XXX.XXX"

SHELL ["/bin/bash", "-c"]

# Environment Variable
ENV DEBIAN_FRONTEND=noninteractive

ENV SATURN_ROOT=/opt/saturn
ENV SATURN_SGL=$SATURN_ROOT/sgl
ENV SATURN_SBL=$SATURN_ROOT/sbl
ENV SATURN_JOENGINE=$SATURN_ROOT/joengine

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

ENV PROGRAM_PREFIX=sh-elf-
ENV NCPU=1
ENV CREATEINSTALLER="NO"

ENV TARGETMACH=sh-elf
ENV BUILDMACH=i686-pc-linux-gnu
ENV HOSTMACH=i686-pc-linux-gnu
ENV OBJFORMAT=ELF

# Add FLAGS
ENV BINUTILS_CFLAGS="-s"
ENV GCC_BOOTSTRAP_FLAGS="--with-cpu=m2"
ENV GCC_FINAL_FLAGS="--with-cpu=m2 --with-sysroot=$SYSROOTDIR"


# Core Packages
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
  gettext \
  autogen \
	autoconf-archive \
	genisoimage \
	--no-install-recommends && \
	apt autoremove -y && \
	## Make sure we leave any X11 related library behind
	apt-get purge -y 'libx11*' x11-common libxt6 && \
	apt autoremove -y && \
	rm -r /var/lib/apt/lists/*

# Base Directories
RUN mkdir -p "${SATURN_ROOT}" "${SATURN_SGL}" "${SATURN_SBL}" "${SATURN_JOENGINE}" "${SATURN_TMP}"  && \
	chmod -R 777 "$SATURN_ROOT" && \
	chmod -R 777 "$SATURN_SGL" && \
	chmod -R 777 "$SATURN_SBL" && \
	chmod -R 777 "$SATURN_JOENGINE" && \
  chmod -R 777 "$SATURN_TMP"

WORKDIR "${SATURN_TMP}"

# Install base tools

RUN git clone https://github.com/willll/Saturn-SDK-GCC-SH2.git "$SATURN_TMP"

RUN chmod 777 *.sh

# Clean up Windows's mess
RUN dos2unix *

RUN chmod +x *.sh

RUN . ./versions.sh && \
 ./build-elf.sh

RUN rm -rf "$SATURN_TMP"

# Set GCC flags
ENV CXXFLAGS=""
ENV LD_LIBRARY_PATH="${INSTALLDIR}/lib:${LD_LIBRARY_PATH}"
ENV CPATH="${INSTALLDIR}/include:${CPATH}"
ENV COMPILER_PATH="${INSTALLDIR}/include:${COMPILER_PATH}"
ENV C_INCLUDE_PATH="${INSTALLDIR}/include:${C_INCLUDE_PATH}"
ENV CPLUS_INCLUDE_PATH="${INSTALLDIR}/include:${CPLUS_INCLUDE_PATH}"
ENV MAKEFLAGS="-j$(nproc)"
ENV CC=${INSTALLDIR}/bin/${PROGRAM_PREFIX}gcc
ENV CXX=${INSTALLDIR}/bin/${PROGRAM_PREFIX}g++


# Set PATH to access compilers
ENV PATH="${INSTALLDIR}/bin:${PATH}"

# Install SGL
RUN git clone https://github.com/SaturnSDK/Saturn-SDK-SGL.git "$SATURN_TMP"
WORKDIR "${SATURN_TMP}"
RUN DOWNLOADDIR=$SATURN_TMP SRCDIR=$SATURN_TMP BUILDDIR=$SATURN_SGL \
  INSTALLDIR=$SATURN_SGL SATURNSDK=$SATURN_ROOT PROGRAM_PREFIX=$PROGRAM_PREFIX \
  bash -c "$SATURN_TMP/download.sh && $SATURN_TMP/extract.sh && \
    $SATURN_TMP/convert.sh && $SATURN_TMP/copyheaders.sh"

# Install SBL
#RUN git clone https://github.com/shicky256/saturndevsbl.git "$SATURN_SBL"
COPY Resources/dl-sbl6.sh $SATURN_TMP
RUN $SATURN_TMP/dl-sbl6.sh
COPY Resources/sbl6 $SATURN_TMP/sbl6_
COPY Resources/build-sbl6.sh $SATURN_TMP
COPY Resources/sbl6.patch $SATURN_TMP
RUN $SATURN_TMP/build-sbl6.sh

ENV SEGALIB=${SATURN_SBL}/segalib
ENV SEGASMP=${SATURN_SBL}/segasmp

# Install Jo Engine
#RUN git clone https://github.com/johannes-fetz/joengine.git "$SATURN_JOENGINE"

# Clean up temporay files
#RUN rm -rf "$SATURN_TMP"



# Set Volume and Workdir
VOLUME /saturn
WORKDIR /saturn

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

CMD ["/bin/bash"]
