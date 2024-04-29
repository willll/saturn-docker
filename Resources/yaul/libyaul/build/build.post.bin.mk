THIS_FILE:=$(firstword $(MAKEFILE_LIST))

ifeq ($(strip $(SH_BUILD_DIR)),)
  $(error Empty SH_BUILD_DIR (SH build directory))
endif

ifeq ($(strip $(SH_OUTPUT_DIR)),)
  $(error Empty SH_OUTPUT_DIR (SH output directory))
endif

ifeq ($(strip $(SH_PROGRAM)),)
  $(error Empty SH_PROGRAM (SH program name))
endif

$(shell mkdir -p $(SH_BUILD_DIR))
$(shell mkdir -p $(SH_OUTPUT_DIR))

# $1 -> Path to asset file
# $2 -> Valid C symbol asset name
define macro-builtin-asset-rule
$(call macro-convert-build-path,$(addsuffix .o,$1)): $1
	@printf -- "$(V_BEGIN_CYAN)$(strip $1)$(V_END)\n"
	$(ECHO)$(YAUL_INSTALL_ROOT)/bin/bin2o $1 $2 $$@

SH_SRCS+= $(addsuffix .o,$1)
endef

$(foreach BUILTIN_ASSET,$(BUILTIN_ASSETS), \
	$(eval $(call macro-builtin-asset-rule,\
		$(call macro-word-split,$(BUILTIN_ASSET),1), \
		$(call macro-word-split,$(BUILTIN_ASSET),2))))

# Check that SH_SRCS don't include duplicates. Be mindful that sort remove
# duplicates
SH_SRCS_UNIQ= $(sort $(SH_SRCS))

SH_SRCS_C:= $(filter %.c,$(SH_SRCS_UNIQ))
SH_SRCS_CXX:= $(filter %.cxx,$(SH_SRCS_UNIQ)) \
	$(filter %.cpp,$(SH_SRCS_UNIQ)) \
	$(filter %.cc,$(SH_SRCS_UNIQ)) \
	$(filter %.C,$(SH_SRCS_UNIQ))
SH_SRCS_S:= $(filter %.sx,$(SH_SRCS_UNIQ))
SH_SRCS_OTHER:= $(filter-out %.c %.cxx %.cpp %.cc %.C %.sx,$(SH_SRCS_UNIQ))

SH_OBJS_UNIQ:= $(addsuffix .o,$(foreach SRC,$(SH_SRCS_C) $(SH_SRCS_CXX) $(SH_SRCS_S) $(SH_SRCS_OTHER),$(basename $(SRC))))
SH_OBJS_UNIQ:= $(foreach OBJ,$(SH_OBJS_UNIQ),$(call macro-convert-build-path,$(OBJ)))

SH_LDFLAGS+= $(SH_DEFSYMS)

ifeq ($(strip $(SH_SPECS)),)
  SH_SPECS:= $(YAUL_INSTALL_ROOT)/$(YAUL_ARCH_SH_PREFIX)/lib/yaul.specs \
		$(YAUL_INSTALL_ROOT)/$(YAUL_ARCH_SH_PREFIX)/lib/yaul-main.specs
endif

# If there are any C++ files, add the specific C++ specs file. This is done to
# avoid adding (small) bloat to any C-only projects
ifneq ($(strip $(SH_SRCS_CXX)),)
  SH_CXX_SPECS:= $(YAUL_INSTALL_ROOT)/$(YAUL_ARCH_SH_PREFIX)/lib/yaul-main-c++.specs
endif

SH_DEPS:= $(SH_OBJS_UNIQ:.o=.d)
SH_TEMPS:= $(SH_OBJS_UNIQ:.o=.i) $(SH_OBJS_UNIQ:.o=.ii) $(SH_OBJS_UNIQ:.o=.s)

# Parse out included paths from GCC when the specs files are used. This is used
# to explictly populate each command database entry with include paths
SH_SYSTEM_INCLUDE_DIRS:=$(shell echo | $(SH_CC) -E -Wp,-v - 2>&1 | \
	awk '/^\s/ { sub(/^\s+/,""); gsub(/\\/,"/"); print }')

# $1 -> $<
define macro-sh-generate-cdb-rule
generate-cdb::
	$(ECHO)printf -- "C\n" >&2
	$(ECHO)printf -- "/usr/bin/gcc$(EXE_EXT)\n" >&2
	$(ECHO)printf -- "$(abspath $(1))\n" >&2
	$(ECHO)printf -- "-D__INTELLISENSE__ -m32 -nostdlibinc -Wno-gnu-statement-expression $(SH_CFLAGS) $(foreach dir,$(SH_SYSTEM_INCLUDE_DIRS),-isystem $(abspath $(dir))) $(foreach dir,$(SHARED_INCLUDE_DIRS),-isystem $(abspath $(dir))) --include="$(YAUL_INSTALL_ROOT)/$(YAUL_PROG_SH_PREFIX)/include/intellisense.h" -c $(abspath $(1))\n" >&2
endef

# $2 -> Build type (release, debug)
define macro-sh-c++-generate-cdb-rule
generate-cdb::
	$(ECHO)printf -- "C++\n" >&2
	$(ECHO)printf -- "/usr/bin/g++$(EXE_EXT)\n" >&2
	$(ECHO)printf -- "$(abspath $(1))\n" >&2
	$(ECHO)printf -- "-D__INTELLISENSE__ -m32 -nostdinc++ \
	-nostdlibinc -Wno-gnu-statement-expression $(SH_CXXFLAGS) \
	$(foreach dir,$(SH_SYSTEM_INCLUDE_DIRS),-isystem $(abspath $(dir))) \
	$(foreach dir,$(SHARED_INCLUDE_DIRS),-isystem $(abspath $(dir))) \
	--include="$(YAUL_INSTALL_ROOT)/$(YAUL_PROG_SH_PREFIX)/include/intellisense.h" -c $(abspath $(1))\n" >&2
endef

# $1 -> $<
# $2 -> $@
define macro-generate-sh-build-object
$2: $1
	@printf -- "$(V_BEGIN_YELLOW)$1$(V_END)\n"
	$(ECHO)$(SH_CC) -MT $2 -MF $(addsuffix .d,$(basename $2)) -MD $(SH_CFLAGS) $(foreach specs,$(SH_SPECS),-specs=$(specs)) -c -o $2 $1
endef

# $1 -> $<
# $2 -> $@
define macro-generate-sh-build-asm-object
$2: $1
	@printf -- "$(V_BEGIN_YELLOW)$1$(V_END)\n"
	$(ECHO)$(SH_CC) $(SH_CFLAGS) -c -o $2 $1
endef

# $1 -> $<
# $2 -> $@
define macro-generate-sh-build-c++-object
$2: $1
	@printf -- "$(V_BEGIN_YELLOW)$1$(V_END)\n"
	$(ECHO)$(SH_CXX) -MT $2 -MF $(addsuffix .d,$(basename $2)) -MD $(SH_CXXFLAGS) $(foreach specs,$(SH_SPECS),-specs=$(specs)) $(foreach specs,$(SH_CXX_SPECS),-specs=$(specs)) -c -o $2 $1
endef

OUTPUT_FILES?=

ifeq ($(strip $(OUTPUT_FILES)),)
OUTPUT_FILES= $(SH_OUTPUT_PATH)/$(SH_PROGRAM).bin
CLEAN_OUTPUT_FILES?= $(OUTPUT_FILES)

$(SH_OUTPUT_PATH)/$(SH_PROGRAM).bin: $(SH_BUILD_PATH)/$(SH_PROGRAM).bin
	$(ECHO)cp $< $@
endif

.build: $(OUTPUT_FILES)

$(SH_BUILD_PATH)/$(SH_PROGRAM).bin: $(SH_BUILD_PATH)/$(SH_PROGRAM).elf
	@printf -- "$(V_BEGIN_YELLOW)$(@F)$(V_END)\n"
	$(ECHO)$(SH_OBJCOPY) -O binary $< $@
	@[ -z "${SILENT}" ] && du -hs $@ | awk '{ print $$1; }' || true

$(SH_BUILD_PATH)/$(SH_PROGRAM).elf: $(SH_OBJS_UNIQ)
	@printf -- "$(V_BEGIN_YELLOW)$(@F)$(V_END)\n"
	$(ECHO)$(SH_LD) $(foreach specs,$(SH_SPECS),-specs=$(specs)) $(foreach specs,$(SH_CXX_SPECS),-specs=$(specs)) $(SH_OBJS_UNIQ) $(SH_LDFLAGS) -o $@
	$(ECHO)$(SH_NM) $(SH_BUILD_PATH)/$(SH_PROGRAM).elf > $(SH_BUILD_PATH)/$(SH_PROGRAM).sym
	$(ECHO)$(SH_OBJDUMP) -S $(SH_BUILD_PATH)/$(SH_PROGRAM).elf > $(SH_BUILD_PATH)/$(SH_PROGRAM).asm

$(foreach SRC,$(SH_SRCS_C), \
	$(eval $(call macro-generate-sh-build-object,$(SRC),\
		$(call macro-convert-build-path,$(addsuffix .o,$(basename $(SRC)))))))

$(foreach SRC,$(SH_SRCS_CXX), \
	$(eval $(call macro-generate-sh-build-c++-object,$(SRC),\
		$(call macro-convert-build-path,$(addsuffix .o,$(basename $(SRC)))))))

$(foreach SRC,$(SH_SRCS_S), \
	$(eval $(call macro-generate-sh-build-asm-object,$(SRC),\
		$(call macro-convert-build-path,$(addsuffix .o,$(basename $(SRC)))))))

$(foreach FILE,$(SH_SRCS_C),$(eval $(call macro-sh-generate-cdb-rule,$(FILE))))
$(foreach FILE,$(SH_SRCS_CXX),$(eval $(call macro-sh-c++-generate-cdb-rule,$(FILE))))

clean:
	$(ECHO)printf -- "$(V_BEGIN_CYAN)$(SH_PROGRAM)$(V_END) $(V_BEGIN_GREEN)clean$(V_END)\n"
	$(ECHO)-rm -f \
	    $(SH_OBJS_UNIQ) \
	    $(SH_DEPS) \
	    $(SH_TEMPS) \
	    $(SH_BUILD_PATH)/$(SH_PROGRAM).asm \
	    $(SH_BUILD_PATH)/$(SH_PROGRAM).bin \
	    $(SH_BUILD_PATH)/$(SH_PROGRAM).elf \
	    $(SH_BUILD_PATH)/$(SH_PROGRAM).map \
	    $(SH_BUILD_PATH)/$(SH_PROGRAM).sym \
	    $(CLEAN_OUTPUT_FILES)

-include $(SH_DEPS)

undefine macro-builtin-asset-rule
undefine macro-generate-sh-build-object
undefine macro-generate-sh-build-asm-object
undefine macro-generate-sh-build-c++-object
