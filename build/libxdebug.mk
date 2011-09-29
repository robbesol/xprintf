# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

# set up some vars

# Target library name (without extension).
# For libary 'libXXX.a' use TARGET_NAME = XXX
TARGET_NAME = xdebug

include ../make/vars.mk

INCLUDEDIRS += ../include .

## where source files live
XDEBUG_SRC_DIR = ../xdebug

SRC += $(XDEBUG_SRC_DIR)/debug.c
SRC += $(XDEBUG_SRC_DIR)/debugfixed3dec.c
SRC += $(XDEBUG_SRC_DIR)/vdebug.c
SRC += $(XDEBUG_SRC_DIR)/debug-channel.c

CHECKUSEDSYMBOLS_DUMMIES_C = xprintf-dummies.c 
#CHECKUSEDSYMBOLS_DUMMIES_C += defaultlibs-dummies.c
include $(MK_DIR)staticlib.mk