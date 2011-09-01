#
# This file sets up common vars for .mk files.
#
#
# NB:
# For local, private adaptations, e.g. different paths to utilities, create a file
#	vars-local.mk
# If present, it will be included at the end of this makefile.
#
#
# Usage:
# Include in your Makefile, near the top
#=====
# include ../../mk/vars.mk
#=====
# This will build for the hardware model with the same name as your current directory.
# If the current directory (basename) is NOT the model name, define the model explicitly:
#=====
# HW_MODEL = Puffin
#=====
#
# Then, define your include directories, C sources
#===== 
# INCLUDEDIRS += path/to/include
# SRC += path/to/my.c
#=====
#
# Last, include the .mk of your choice, e.g. for a flashable binary:
#===== 
# $(call INCLUDE_COMMON_MK flash.mk)
#===== 
#

#===== testing make type
## $ make -v
## GNU Make 3.80 [...]
_MAKE_V := $(strip $(shell make -v))
_MAKE_V1 := $(firstword $(_MAKE_V))
ifneq (GNU,$(_MAKE_V1))
$(error wrong make: need GNU, got: $(_MAKE_V))
endif

# default target:
this::

#--------------------------------------------------------------------------
#--------------------------------------------------------------------------
#----- No user servicable parts inside.

ifeq (,$(strip $(HW_MODEL)))
HW_MODEL = $(notdir $(CURDIR))
endif

ifeq (,$(strip $(HW_MODEL_NODBG)))
HW_MODEL_NODBG = $(patsubst %-dbg,%,$(HW_MODEL))
endif

ifeq (,$(strip $(_MK_VARS_PATH)))
_MK_VARS_PATH := $(lastword $(MAKEFILE_LIST))
MK_DIR := $(patsubst %/,%,$(dir $(_MK_VARS_PATH)))
PROJPARENT_DIR := ..
endif

ifeq (,$(strip $(BASE_PROJECT_MODEL_DIR)))
BASE_PROJECT_MODEL_DIR := $(patsubst %/,%,$(dir $(firstword $(MAKEFILE_LIST))))
endif
ifeq (,$(strip $(BASE_PROJECT_ROOT_DIR_SLASH)))
BASE_PROJECT_ROOT_DIR_SLASH :=  $(if $(filter .,$(BASE_PROJECT_MODEL_DIR)),../,$(dir $(BASE_PROJECT_MODEL_DIR)))
endif
ifeq (,$(strip $(BASE_PROJECT_NODBG_DIR)))
BASE_PROJECT_NODBG_DIR := $(BASE_PROJECT_ROOT_DIR_SLASH)$(HW_MODEL_NODBG)
endif

# This is a way to get the directory of the current makefile, without its trailing slash,
# provided that you use a ':=' type of assignment.

_MAKEFILE_LIST_CLEAN = $(filter-out obj/%,$(filter-out $(MK_DIR)/%,$(MAKEFILE_LIST)))
_THIS_MAKEFILE_PATH = $(lastword $(_MAKEFILE_LIST_CLEAN))
_THIS_MAKEFILE_NAME = $(notdir $(_THIS_MAKEFILE_PATH))
_THIS_MAKEFILE_DIR = $(patsubst %/,%,$(dir $(_THIS_MAKEFILE_PATH)))
_THIS_MAKEFILE_PARENTDIR = $(if $(filter .,$(_THIS_MAKEFILE_DIR)),..,$(patsubst %/,%,$(dir $(_THIS_MAKEFILE_DIR))))

_THIS_PROJECT_MODEL_DIR = $(_THIS_MAKEFILE_PARENTDIR)/$(HW_MODEL)
_THIS_PROJECT_NODBG_DIR = $(_THIS_MAKEFILE_PARENTDIR)/$(HW_MODEL_NODBG)

_THIS_MAKEFILE_NODBG = $(_THIS_PROJECT_NODBG_DIR)/$(_THIS_MAKEFILE_NAME)

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#CC_DEBUG_FLAGS ?= -gstabs
#CC_DEBUG_FLAGS ?= -ggdb
CC_DEBUG_FLAGS ?= -g

# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
CC_OPTIM_FLAGS ?= -Os

# MCU name and submodel
MCU = arm7tdmi
SUBMDL = AT91SAM7S256

## Exception-Vector placement only supported for "ROM_RUN"
## (placement settings ignored when using "RAM_RUN")
## - Exception vectors in ROM:
#VECTOR_LOCATION = VECTORS_IN_ROM
## - Exception vectors in RAM:
VECTOR_LOCATION ?= VECTORS_IN_RAM

# Define programs and commands.
CPPFLAGS =
CC = arm-elf-gcc
CFLAGS =
CCVERSION = $(CC) --version

AS = $(CC) -x assembler-with-cpp
ASFLAGS =

LDFLAGS =
LDLIBS =
LIBS_REQUIRED_LINK_ORDER :=
LIBS_REQUIRED_BUILD_ORDER :=
_DEBUG = Start

define _ADD_TO_LIBS_REQUIRED
ifeq (,$(filter $1,$(LIBS_REQUIRED_LINK_ORDER)))
LIBS_REQUIRED_LINK_ORDER := $1 $(LIBS_REQUIRED_LINK_ORDER)
LIBS_REQUIRED_BUILD_ORDER += $1
LDFLAGS += -u CompiledString_$(subst -,_,$(patsubst lib%,%,$(basename $(notdir $1))))
endif
endef

define LIB_REQUIRE
$(eval $(call _ADD_TO_LIBS_REQUIRED,$(strip $1)))
endef

define LINK_WITH_MY_LIB_PATH
ifeq (,$(filter $1,$(LIBS_REQUIRED_LINK_ORDER)))
$(if $(MK_TRACE_MAKEFILES),$(shell echo >&2 "+++ LINK_WITH_MY_LIB_PATH($1): libsrequired.mk"))
$(eval -include $(dir $1)libsrequired.mk)
$(if $(MK_TRACE_MAKEFILES),$(shell echo >&2 "+++ LINK_WITH_MY_LIB_PATH($1): _ADD_TO_LIBS_REQUIRED"))
$(eval $(call _ADD_TO_LIBS_REQUIRED,$1))
$1:
	$(MAKE) -C $(dir $1) $(notdir $1)
else
$(if $(MK_TRACE_MAKEFILES),$(shell echo >&2 "--- LINK_WITH_MY_LIB_PATH($1)"))
endif
endef

define LINK_WITH_MY_LIB
$(eval $(call LINK_WITH_MY_LIB_PATH,$(_THIS_PROJECT_MODEL_DIR)/lib$(strip $1)-$(HW_MODEL).a))
endef

define _INCLUDE_ONCE
$(if $(MK_TRACE_MAKEFILES),$(if $(filter $1,$(MAKEFILE_LIST)),,$(shell echo >&2 "+++ $1"))) \
$(if $(filter $1,$(MAKEFILE_LIST)),,$(eval include $1))
endef

define INCLUDE_ONCE
$(call _INCLUDE_ONCE,$(strip $1))
endef

define INCLUDE_MK
$(call INCLUDE_ONCE,$(PROJPARENT_DIR)/$(strip $1))
endef

define INCLUDE_COMMON_MK
$(call INCLUDE_ONCE,$(MK_DIR)/$(strip $1))
endef

AR = arm-elf-ar
# AS flags: rv is default, s indicates running ranlib
ARFLAGS = rvs

SIZE = arm-elf-size
NM = arm-elf-nm
STRINGS = arm-elf-strings

OBJDUMP = arm-elf-objdump
OBJCOPY = arm-elf-objcopy
OBJPREFIX = obj/dot/dot/

FLASH_EXEC = $(PROJPARENT_DIR)/flash/flash

GENERATED_SRC_DIR = src-generated/

#==================================================

-include vars-local.mk

##==================================================
##
## Here is a table of some of the more common variables used as names of programs in built-in rules: makefiles. 
##
## AR Archive-maintaining program; default "ar". 
## AS Program for compiling assembly files; default "as". 
## CC Program for compiling C programs; default "cc". 
## CO Program for checking out files from RCS; default "co". 
## CXX Program for compiling C++ programs; default "g++". 
## CO Program for extracting a file from RCS; default "co". 
## CPP Program for running the C preprocessor, with results to standard output; default "$(CC) -E". 
## FC Program for compiling or preprocessing Fortran and Ratfor programs; default "f77". 
## GET Program for extracting a file from SCCS; default "get". 
## LEX Program to use to turn Lex grammars into source code; default "lex". 
## YACC Program to use to turn Yacc grammars into source code; default "yacc". 
## LINT Program to use to run lint on source code; default "lint". 
## M2C Program to use to compile Modula-2 source code; default "m2c". 
## PC Program for compiling Pascal programs; default "pc". 
## MAKEINFO Program to convert a Texinfo source file into an Info file; default "makeinfo". 
## TEX Program to make TEX dvi files from TEX source; default "tex". 
## TEXI2DVI Program to make TEX dvi files from Texinfo source; default "texi2dvi".
## WEAVE Program to translate Web into TEX; default "weave". 
## CWEAVE Program to translate C Web into TEX; default "cweave". 
## TANGLE Program to translate Web into Pascal; default "tangle". 
## CTANGLE Program to translate C Web into C; default "ctangle". 
## RM Command to remove a file; default "rm -f". 
##
## Here is a table of variables whose values are additional arguments for the programs 
## above. The default values for all of these is the empty string, unless otherwise noted. 
##
## ARFLAGS Flags to give the archive-maintaining program; default "rv". 
## ASFLAGS Extra flags to give to the assembler (when explicitly invoked on a ".s" or ".S" file). 
## CFLAGS Extra flags to give to the C compiler. 
## CXXFLAGS Extra flags to give to the C++ compiler. 
## COFLAGS Extra flags to give to the RCS co program. 
## CPPFLAGS Extra flags to give to the C preprocessor and programs that use it (the C and Fortran compilers). 
## FFLAGS Extra flags to give to the Fortran compiler. 
## GFLAGS Extra flags to give to the SCCS get program. 
## LDFLAGS Extra flags to give to compilers when they are supposed to invoke the linker, "ld". 
## LFLAGS Extra flags to give to Lex. 
## YFLAGS Extra flags to give to Yacc. 
## PFLAGS Extra flags to give to the Pascal compiler. 
## RFLAGS Extra flags to give to the Fortran compiler for Ratfor programs. 
## LINTFLAGS Extra flags to give to lint. 
##
##==================================================
##
## "n.o" is made automatically from "n.c" with a command of the form 
## 	$(CC) -c $(CPPFLAGS) $(CFLAGS)
## "n.o" is made automatically from "n.s" by running the assembler, as. The precise command is 
## 	$(AS) $(ASFLAGS)
## "n.s" is made automatically from "n.S" by running the C preprocessor, cpp. The precise command is 
## 	$(CPP) $(CPPFLAGS)
## "n " is made automatically from "n.o" by running the linker (usually called ld) via the C compiler. The precise command used is 
## 	$(CC) $(LDFLAGS) n.o $(LOADLIBES) $(LDLIBS)
##
##==================================================
