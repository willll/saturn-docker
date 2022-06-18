ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

ifneq (1,$(words [$(strip $(YAUL_INSTALL_ROOT))]))
  $(error YAUL_INSTALL_ROOT (install root directory) contains spaces)
endif

ifeq ($(strip $(YAUL_ARCH_SH_PREFIX)),)
  $(error Undefined YAUL_ARCH_SH_PREFIX (tool-chain prefix))
endif

ifneq (1,$(words [$(strip $(YAUL_ARCH_SH_PREFIX))]))
  $(error YAUL_ARCH_SH_PREFIX (tool-chain prefix) contains spaces)
endif

ifneq (1,$(words [$(strip $(YAUL_PROG_SH_PREFIX))]))
  $(error YAUL_PROG_SH_PREFIX (tool-chain program prefix) contains spaces)
endif

ifeq ($(strip $(YAUL_CDB)),)
  $(error Undefined YAUL_CDB (update JSON compile command database))
endif

# Check options
ifeq ($(strip $(YAUL_OPTION_DEV_CARTRIDGE)),)
  $(error Undefined YAUL_OPTION_DEV_CARTRIDGE (development cartridge option))
endif

ifeq ($(strip $(YAUL_OPTION_BUILD_ASSERT)),)
  $(error Undefined YAUL_OPTION_BUILD_ASSERT (build ASSERT))
endif
ifneq ($(YAUL_OPTION_BUILD_ASSERT),$(filter $(YAUL_OPTION_BUILD_ASSERT),0 1))
  $(error Invalid value for YAUL_OPTION_BUILD_ASSERT (build assert))
endif

ifeq ($(strip $(SILENT)),)
  ECHO=
else
  ECHO=@
endif
export ECHO

ifeq ($(strip $(NOCOLOR)),)
V_BEGIN_BLACK= [1;30m
V_BEGIN_RED= [1;31m
V_BEGIN_GREEN= [1;32m
V_BEGIN_YELLOW= [1;33m
V_BEGIN_BLUE= [1;34m
V_BEGIN_MAGENTA= [1;35m
V_BEGIN_CYAN= [1;36m
V_BEGIN_WHITE= [1;37m
V_END= [m
else
V_BEGIN_BLACK=
V_BEGIN_RED=
V_BEGIN_GREEN=
V_BEGIN_YELLOW=
V_BEGIN_BLUE=
V_BEGIN_MAGENTA=
V_BEGIN_CYAN=
V_BEGIN_WHITE=
V_END=
endif
export V_BEGIN_YELLOW

# Get the Makefile that calls to include this Makefile
BUILD_ROOT:= $(patsubst %/,%,$(dir $(abspath $(firstword $(MAKEFILE_LIST)))))

ifeq '$(OS)' "Windows_NT"
EXE_EXT:= .exe

# In order to avoid the following error under MSYS2 and MinGW-w32:
#
# /opt/tool-chains/sh2eb-elf/bin/sh2eb-elf-gcc.exe: error while loading shared
# libraries: libwinpthread-1.dll: cannot open shared object file: No such file
# or directory
#
# We need to have /mingw/bin in our path. It's unclear exactly why, but
# libwinpthread-1.dll resides in /mingw64/bin. Copying libwinpthread-1.dll to
# /opt/tool-chains/sh2eb-elf/bin does not resolve the issue
PATH:= /mingw64/bin:$(PATH)
endif

# $1 -> Relative/absolute path to be converted using '@' instead of '/'
define macro-convert-build-path
$(SH_BUILD_PATH)/$(subst /,@,$(abspath $1))
endef

# $1 -> Variable
# $2 -> Index
define macro-word-split
$(word $2,$(subst ;, ,$1))
endef

# Customizable (must be overwritten in user's Makefile)
SH_PROGRAM?= unknown-program
SH_SRCS?=
SH_SRCS_NO_LINK?=
SH_LIBRARIES?=
SH_BUILD_DIR?= build
IMAGE_DIRECTORY?= cd
IMAGE_1ST_READ_BIN?= A.BIN

YAUL_INCLUDES_PATH=$(YAUL_INSTALL_ROOT)/sh-elf/include/
YAUL_RELASE_LIB_PATH=$(YAUL_INSTALL_ROOT)/sh-elf/lib/
YAUL_DEBUG_LIB_PATH=$(YAUL_INSTALL_ROOT)/sh-elf/lib/

YAUL_PROG_SH_PREFIX?= $(YAUL_ARCH_SH_PREFIX)
ifeq ($(strip $(YAUL_PROG_SH_PREFIX)),)
YAUL_PROG_SH_PREFIX:= $(YAUL_ARCH_SH_PREFIX)
endif

SH_AS:=      $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-as$(EXE_EXT)
SH_AR:=      $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-gcc-ar$(EXE_EXT)
SH_CC:=      $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-gcc$(EXE_EXT)
SH_CXX:=     $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-g++$(EXE_EXT)
SH_LD:=      $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-gcc$(EXE_EXT)
SH_NM:=      $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-gcc-nm$(EXE_EXT)
SH_OBJCOPY:= $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-objcopy$(EXE_EXT)
SH_OBJDUMP:= $(YAUL_SH_TOOLCHAIN)/bin/$(YAUL_PROG_SH_PREFIX)-objdump$(EXE_EXT)

M68K_AS:=      $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-as$(EXE_EXT)
M68K_AR:=      $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-gcc-ar$(EXE_EXT)
M68K_CC:=      $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-gcc$(EXE_EXT)
M68K_CXX:=     $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-g++$(EXE_EXT)
M68K_LD:=      $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-gcc$(EXE_EXT)
M68K_NM:=      $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-gcc-nm$(EXE_EXT)
M68K_OBJCOPY:= $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-objcopy$(EXE_EXT)
M68K_OBJDUMP:= $(YAUL_68K_TOOLCHAIN)/bin/$(YAUL_ARCH_M68K_PREFIX)-objdump$(EXE_EXT)

SH_AFLAGS= --fatal-warnings
SH_CFLAGS= \
	-W \
	-Wall \
	-Wextra \
	-Werror \
	-Wunused-parameter \
	-Wstrict-aliasing \
	-Wno-main \
	-Wno-format \
	-save-temps=obj \
	-DHAVE_DEV_CARTRIDGE=$(YAUL_OPTION_DEV_CARTRIDGE) \
	-DHAVE_GDB_SUPPORT=$(YAUL_OPTION_BUILD_GDB) \
	-DHAVE_ASSERT_SUPPORT=$(YAUL_OPTION_BUILD_ASSERT)

SH_LDFLAGS= \
	-Wl,--gc-sections \
	-Wl,-Map,$(SH_BUILD_PATH)/$(SH_PROGRAM).map

SH_CXXFLAGS= $(SH_CFLAGS)
SH_LXXFLAGS= $(SH_LDFLAGS)

SH_BUILD_PATH:= $(abspath $(SH_BUILD_DIR))

SH_SPECS:= $(YAUL_INSTALL_ROOT)/$(YAUL_ARCH_SH_PREFIX)/lib/yaul.specs \
					$(YAUL_INSTALL_ROOT)/$(YAUL_ARCH_SH_PREFIX)/lib/yaul-main.specs


IP_VERSION?= V1.000
IP_RELEASE_DATE?= YYYYMMDD
IP_AREAS?= JTUBKAEL
IP_PERIPHERALS?= JAMKST
IP_TITLE?= Title
IP_MASTER_STACK_ADDR?= 0x06004000
IP_SLAVE_STACK_ADDR?= 0x06002000
IP_1ST_READ_ADDR?= 0x06004000
IP_1ST_READ_SIZE?= 0

CDB_FILE:= compile_commands.json
CDB_GCC?= /usr/bin/gcc
CDB_CPP?= /usr/bin/g++

.PHONY: all clean list-targets

# The targets which .SECONDARY depends on are treated as intermediate files,
# except that they are never automatically deleted
.SECONDARY: pre-build-iso post-build-iso build

.SUFFIXES:
.SUFFIXES: .c .cc .C .cpp .cxx .sx .o .bin .elf

.PRECIOUS: %.elf %.c %.o

all: build

pre-build-iso:

post-build-iso:
