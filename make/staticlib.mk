# Hey Emacs, this is a -*- makefile -*-
#

ifeq (,$(strip $(MK_DIR)))
$(error vars.mk not yet included)
endif

ifeq (,$(strip $(TARGET_NAME)))
$(error No TARGET_NAME defined)
endif
LIBRARY_A = lib$(TARGET_NAME).a

include $(MK_DIR)compilationid.mk
SRC += $(COMPILATIONID_FILE_C)
_COMPILATIONID_FILE_O = $(addprefix $(OBJPREFIX),$(COMPILATIONID_FILE_C:.c=.o))

include $(MK_DIR)cso.mk

ifeq (,$(strip $(ALLOBJ)))
define _NO_SOURCE_ERROR_TEXT
No sources defined. Use the following variable:
  SRC += dir/file.c
endef
$(error $(_NO_SOURCE_ERROR_TEXT))
endif

# default target:
.PHONY: all this
all this:: lib

.PHONY: clean cleanall
cleanall:: clean

.PHONY: lib _lib_announce
_lib_announce:
	@echo
	@echo "-------- start $(LIBRARY_A) --------"
lib: _lib_announce $(LIBRARY_A)
	-@$(call IDENT,$(LIBRARY_A))
	@echo "-------- end $(LIBRARY_A) --------"
	@echo

# Link: create ELF output file from object files.
_CHECKUSEDSYMBOLS_ALLDUMMIES_C = $(CHECKUSEDSYMBOLS_DUMMIES_C) $(CHECKUSEDSYMBOLS_STARTFILE_C)

_CHECKUSEDSYMBOLS_ALLDUMMIES_O = $(addprefix $(OBJPREFIX),$(_CHECKUSEDSYMBOLS_ALLDUMMIES_C:.c=.o))
$(_CHECKUSEDSYMBOLS_ALLDUMMIES_O): $(MAKEFILE_LIST_NO_DEPS)
$(_CHECKUSEDSYMBOLS_ALLDUMMIES_O): CFLAGS = $(CHECKUSEDSYMBOLS_CFLAGS)

$(LIBRARY_A): $(_CHECKUSEDSYMBOLS_ALLDUMMIES_O) $(ALLOBJ) $(MAKEFILE_LIST_NO_DEPS)
ifneq (,$(strip $(_COMPILATIONID_FILE_O)))
	@echo
	@echo "Creating timestamp for library $@:"
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) $(COMPILATIONID_FILE_C) -o $(_COMPILATIONID_FILE_O)
endif
ifneq (,$(strip $(_CHECKUSEDSYMBOLS_ALLDUMMIES_O)))
	@echo
	@echo "Checking library $@ for unresolved externals and forbidden libc symbols ..."
	@$(CC) $(CHECKUSEDSYMBOLS_CFLAGS) $(CHECKUSEDSYMBOLS_LDFLAGS) $(ALLOBJ) $(CHECKUSEDSYMBOLS_LIBS) $(_CHECKUSEDSYMBOLS_ALLDUMMIES_O) -o /dev/null \
	|| ( echo >&2 "----- check for unresolved externals and forbidden libc symbols failed"; \
		 echo >&2 "----- sources checked: $(ALLOBJ)"; \
		 echo >&2 "----- linked with libraries: $(CHECKUSEDSYMBOLS_LIBS)"; \
		 echo >&2 "----- and with (dummy) symbols in files: $(CHECKUSEDSYMBOLS_DUMMIES_C)"; \
		 exit 1; )
endif
	@echo
	@echo "Creating static library $@:"
	@$(RM) $@
	$(AR) $(ARFLAGS) $@ $(ALLOBJ)
	@echo

#####


.PHONY: list-defines
list-defines: $(ALLOBJ)
	@echo "-------- defined symbols in $(LIBRARY_A) --------"
	@$(NM) -g $(ALLOBJ) | sed -n -e 's=^[0-9a-fA-F]*[ 	][ 	]*T[ 	]*==p' | sort -b
	@echo "-------- end of defined symbols in $(LIBRARY_A) --------"
	@echo

# Target: clean project.
clean::
	$(RM) $(LIBRARY_A)
