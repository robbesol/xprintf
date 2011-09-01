# Hey Emacs, this is a -*- makefile -*-
#

ifeq ($(strip $(PROJPARENT_DIR)),)
$(error vars.mk not yet included)
endif

# the restricted allowed set from -lc
_CHECK_LIBC_DUMMIES ?= defaultlibs-dummies.c


# $(call CHECK_FORBIDDEN_LIBC OBJS, LIBS)
# Checks that undefined symbols in OBJS are resolved by LIBS or by defaultlibs-dummies.c.
# How?
# 1. Link OBJS with LIBS but do not use the default libraries. Instead,
# 2. add -lgcc explicitly and finally
# 3. link against defaultlibs-dummies.c, the restricted allowed set from -lc.
#
# Remember, this is called instead of a command line, so remove all newlines from the definition.

define CHECK_FORBIDDEN_LIBC
$(if $(strip $(1)),,echo >&2 "CHECK_FORBIDDEN_LIBC(): no test objects defined"; exit 1; ) \
test -f "$(_CHECK_LIBC_DUMMIES)" || { echo >&2 "CHECK_FORBIDDEN_LIBC(): no file $(_CHECK_LIBC_DUMMIES)"; exit 1; } ; \
$(CC) $(LDFLAGS) -nodefaultlibs $(1) $(2) -lgcc $(_CHECK_LIBC_DUMMIES) -o /dev/null \
|| ( \
  echo >&2 "----- check for use of forbidden libc items failed"; \
  echo >&2 "----- check used: $(patsubst ../../%,%,$(if $(2), $(2),only) $(_CHECK_LIBC_DUMMIES))"; \
  echo >&2 "----- linker scripts: $(LDSCRIPT) + $(LDSCRIPTINCLUDES)"; \
  $(if $(MK_TRACE_MAKEFILES),echo >&2 "----- Makefile nesting:"; $(foreach M,$(filter-out obj/%,$(_MAKEFILE_LIST_CLEAN)), echo  >&2 "    $M";)) \
  exit 1; \
)
endef
