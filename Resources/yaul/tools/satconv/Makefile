include ../../env.mk

TARGET:= satconv
PROGRAM:= $(TARGET)$(EXE_EXT)

SUB_BUILD:=$(YAUL_BUILD)/tools/$(TARGET)

SRCS:= \
	ezxml.c \
	map.c \
	qdbmp.c \
	satconv.c \
	sprite.c \
	tile.c

CFLAGS:= \
	-DEZXML_NOMMAP \
	-O2 \
	-g \
	-s \
	-std=gnu99 \
	-Wall \
	-Wextra \
	-Wuninitialized \
	-Winit-self \
	-Wuninitialized \
	-Wshadow \
	-Wno-unused \
	-Wno-parentheses \
	-Wno-sign-compare

LDFLAGS=

INCLUDES:=

OBJS:= $(addprefix $(YAUL_BUILD_ROOT)/$(SUB_BUILD)/,$(SRCS:.c=.o))
DEPS:= $(addprefix $(YAUL_BUILD_ROOT)/$(SUB_BUILD)/,$(SRCS:.c=.d))

.PHONY: all clean distclean install

all: $(YAUL_BUILD_ROOT)/$(SUB_BUILD)/$(PROGRAM)

$(YAUL_BUILD_ROOT)/$(SUB_BUILD)/$(PROGRAM): $(YAUL_BUILD_ROOT)/$(SUB_BUILD) $(OBJS)
	@printf -- "$(V_BEGIN_YELLOW)$(shell v="$@"; printf -- "$${v#$(YAUL_BUILD_ROOT)/}")$(V_END)\n"
	$(ECHO)$(X86_CC) -o $@ $(OBJS) $(LDFLAGS)

$(YAUL_BUILD_ROOT)/$(SUB_BUILD):
	$(ECHO)mkdir -p $@

$(YAUL_BUILD_ROOT)/$(SUB_BUILD)/%.o: %.c
	@printf -- "$(V_BEGIN_YELLOW)$(shell v="$@"; printf -- "$${v#$(YAUL_BUILD_ROOT)/}")$(V_END)\n"
	$(ECHO)mkdir -p $(@D)
	$(ECHO)$(X86_CC) -Wp,-MMD,$(YAUL_BUILD_ROOT)/$(SUB_BUILD)/$*.d $(CFLAGS) $(foreach DIR,$(INCLUDES),-I$(DIR)) -c -o $@ $<
	$(ECHO)$(SED) -i -e '1s/^\(.*\)$$/$(subst /,\/,$(dir $@))\1/' $(YAUL_BUILD_ROOT)/$(SUB_BUILD)/$*.d

clean:
	$(ECHO)$(RM) $(OBJS) $(DEPS) $(YAUL_BUILD_ROOT)/$(SUB_BUILD)/$(PROGRAM)

distclean: clean

install: $(YAUL_BUILD_ROOT)/$(SUB_BUILD)/$(PROGRAM)
	@printf -- "$(V_BEGIN_BLUE)$(SUB_BUILD)/$(PROGRAM)$(V_END)\n"
	$(ECHO)mkdir -p $(YAUL_PREFIX)/bin
	$(ECHO)$(INSTALL) -m 755 $< $(YAUL_PREFIX)/bin/

-include $(DEPS)
