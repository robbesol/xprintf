# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

# set up some vars
include vars.mk

# Target file name (without extension).
# For libary 'libXXX.a' use TARGET = XXX
TARGET = peripheral-$(HW_MODEL)

INCLUDEDIRS += ../include

COMMON_SRC_DIR = ../src

SRC += $(COMMON_SRC_DIR)/xprintf-core.c
SRC += $(COMMON_SRC_DIR)/xprintf-double.c
SRC += $(COMMON_SRC_DIR)/xsnprintf.c
SRC += $(COMMON_SRC_DIR)/snprintf.c

#SRC += $(COMMON_SRC_DIR)/xprintf-rsrc.c

include staticlib.mk
