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
# include ../make/vars.mk
#=====
#
# Then, define your include directories, C sources
#===== 
# INCLUDEDIRS += path/to/include
# SRC += path/to/my.c
#=====
#

#===== testing make type
## $ make -v
## GNU Make 3.80 [...]
_MAKE_V := $(strip $(shell make -v))
ifneq (GNU,$(firstword $(_MAKE_V)))
$(error wrong make: need GNU, got: $(_MAKE_V))
endif

# default target:
this::


#--------------------------------------------------------------------------
#--------------------------------------------------------------------------
#----- No user servicable parts inside.

MK_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

OBJPREFIX = obj/dot/dot/
GENERATED_SRC_DIR = src-generated/

#==================================================

# what C dialect to use
# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
#
CSTANDARD ?= -std=c99

CFLAGS += $(CSTANDARD)

# no line wrap for warnings/errors
CFLAGS += -fmessage-length=0

# prevent built-in expanding of printf() to puts()
CFLAGS += -fno-builtin-printf
CFLAGS += -fno-builtin

# treat warnings as errors -- a gentle push in the back to write clean, portable code.
CFLAGS += -Werror

# warnings
CFLAGS += -pedantic
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wformat-extra-args -Wformat-zero-length -Wformat-nonliteral -Wformat-security

CFLAGS += -Wcast-align -Wimplicit 
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wcast-qual
CFLAGS += -Wnested-externs 

CFLAGS += -Wmissing-prototypes 
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-declarations

CFLAGS += -Waggregate-return 

# add include flags
CFLAGS += $(patsubst %,-I%,$(INCLUDEDIRS))


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


# Define programs and commands.
CC = gcc
CCVERSION = $(CC) --version

AR = ar
# AR flags: rv is default, s indicates running ranlib
ARFLAGS = rvs

NM = nm


# Compiler flags to generate dependency files.
### DEPFLAGS += -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
DEPFLAGS += -MD
# Add "phony" targets (empty) for header files. Good for when a header file disappears (rename, obsolete). 
DEPFLAGS += -MP


##==================================================
##
## Here is a table of some of the more common variables used as names of programs in built-in rules: makefiles. 
##
## AR Archive-maintaining program; default "ar". 
## AS Program for compiling assembly files; default "as". 
## CC Program for compiling C programs; default "cc". 
## CPP Program for running the C preprocessor, with results to standard output; default "$(CC) -E". 
## RM Command to remove a file; default "rm -f". 
##
## Here is a table of variables whose values are additional arguments for the programs 
## above. The default values for all of these is the empty string, unless otherwise noted. 
##
## ARFLAGS Flags to give the archive-maintaining program; default "rv". 
## ASFLAGS Extra flags to give to the assembler (when explicitly invoked on a ".s" or ".S" file). 
## CFLAGS Extra flags to give to the C compiler. 
## CPPFLAGS Extra flags to give to the C preprocessor and programs that use it (the C and Fortran compilers). 
## LDFLAGS Extra flags to give to compilers when they are supposed to invoke the linker, "ld". 
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
