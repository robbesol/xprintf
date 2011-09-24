# Hey Emacs, this is a -*- makefile -*-
#

ifeq (,$(strip $(MK_DIR)))
$(error vars.mk not yet included)
endif

ifeq (,$(strip $(TARGET)))
$(error No TARGET defined)
endif

include $(MK_DIR)compilationid.mk

include $(MK_DIR)cso.mk


ifeq (,$(strip $(ALLOBJ)))
define _NO_SOURCE_ERROR_TEXT
No sources defined. Use the following variable:
  SRC += dir/file.c # for ISO-C
endef
$(error $(_NO_SOURCE_ERROR_TEXT))
endif


TARGETLIB = lib$(TARGET).a

# the file with the restricted allowed set of functions from -lc (and other default libraries)
CHECKUSEDSYMBOLS_LIBCDUMMIES_C = defaultlibs-dummies.c

# gcc flags to link with no default libraries, only CHECKUSEDSYMBOLS_LIBCDUMMIES_C and checkusedsymbols-start.c:
_CHECKUSEDSYMBOLS_CFLAGS = $(CSTANDARD) -fno-builtin
_CHECKUSEDSYMBOLS_LDFLAGS = -nostartfiles -nodefaultlibs -static -Xlinker -e -Xlinker _checkusedsymbols_start
_CHECKUSEDSYMBOLS_LIBS = -lgcc

# Listing of phony targets.
.PHONY: clean cleanall all this

# default target:
all:: this

cleanall:: clean

this:: lib

.PHONY: lib _lib_announce
_lib_announce:
	@echo
	@echo "-------- start $(TARGETLIB) --------"
lib: _lib_announce $(TARGETLIB)
	-@$(call IDENT,$(TARGETLIB))
	@echo "-------- end $(TARGETLIB) --------"
	@echo

# Link: create ELF output file from object files.

$(TARGETLIB): $(ALLOBJ) $(MAKEFILE_LIST)
ifneq (,$(strip $(CHECKUSEDSYMBOLS_LIBCDUMMIES_C)))
	@$(MAKE) checklibexternals
endif
	@echo
	@echo "Creating static library $@:"
	@$(RM) $@
	$(AR) $(ARFLAGS) $@ $(ALLOBJ)
	@echo

#####

_CHECKUSEDSYMBOLS_LIBCDUMMIES_O = $(OBJPREFIX)$(CHECKUSEDSYMBOLS_LIBCDUMMIES_C:.c=.o)
$(_CHECKUSEDSYMBOLS_LIBCDUMMIES_O): $(MAKEFILE_LIST)
$(_CHECKUSEDSYMBOLS_LIBCDUMMIES_O): CFLAGS = $(_CHECKUSEDSYMBOLS_CFLAGS)

.PHONY: checklibexternals
checklibexternals::
	@echo
	@echo "Checking library for external symbols ..."
checklibexternals:: $(ALLOBJ) $(_CHECKUSEDSYMBOLS_LIBCDUMMIES_O)
	@$(CC) $(_CHECKUSEDSYMBOLS_CFLAGS) $(_CHECKUSEDSYMBOLS_LDFLAGS) $(ALLOBJ) $(_CHECKUSEDSYMBOLS_LIBS) $(_CHECKUSEDSYMBOLS_LIBCDUMMIES_O) $(MK_DIR)checkusedsymbols-start.c -o /dev/null \
	|| ( echo >&2 "----- check for use of forbidden libc items failed"; \
		 echo >&2 "----- check used: $(patsubst ../%,%,$(_CHECKUSEDSYMBOLS_LIBS) $(CHECKUSEDSYMBOLS_LIBCDUMMIES_C))"; \
		 exit 1; )

.PHONY: list-defines
list-defines: $(ALLOBJ)
	@echo "-------- defined symbols in $(TARGETLIB) --------"
	@$(NM) -g $(ALLOBJ) | sed -n -e 's=^[0-9a-fA-F]*[ 	][ 	]*T[ 	]*==p' | sort -b
	@echo "-------- end of defined symbols in $(TARGETLIB) --------"
	@echo

# Target: clean project.
clean::
	$(RM) $(TARGETLIB) $(TARGETLIB)-*.log
