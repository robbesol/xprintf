# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

# set up some vars

# Target library name (without extension).
# For libary 'libXXX.a' use TARGET_NAME = XXX
TARGET_NAME = xprintf

include ../make/vars.mk

INCLUDEDIRS += ../include

## where source files live
XPRINTF_SRC_DIR = ../xprintf
LIBC_SRC_DIR = ../libc-replacements

SRC += $(XPRINTF_SRC_DIR)/xprintf-core.c
SRC += $(XPRINTF_SRC_DIR)/xprintf-double.c
SRC += $(XPRINTF_SRC_DIR)/xsnprintf.c
SRC += $(LIBC_SRC_DIR)/snprintf.c
$(OBJPREFIX)$(XPRINTF_SRC_DIR)/snprintf.o: CFLAGS += -fno-builtin-snprintf -fno-builtin-vsnprintf

#SRC += $(XPRINTF_SRC_DIR)/xprintf-rsrc.c

CHECKUSEDSYMBOLS_DUMMIES_C = defaultlibs-dummies.c
include $(MK_DIR)staticlib.mk