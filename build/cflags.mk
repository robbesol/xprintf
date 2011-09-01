# Hey Emacs, this is a -*- makefile -*-

# what C dialect to use

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
#
# We need gnu extentions because of in-line asm (Brr!)
CSTANDARD ?= -std=gnu99

CFLAGS += $(CSTANDARD)

# no line wrap for warnings/errors
CFLAGS += -fmessage-length=0

# -fomit-frame-pointer is automatically enabled at optimization levels -O, -O2, -O3, -Os.

# consider this:
#CFLAGS += -mlong-calls 
#CFLAGS += -ffunction-sections

# prevent built-in expanding of printf() to puts()
CFLAGS += -fno-builtin-printf

# un-warnings
CATER_FOR_FORMAT_S_HASH = -Wno-format -Wnonnull

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
