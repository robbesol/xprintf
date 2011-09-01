# Hey Emacs, this is a -*- makefile -*-
#

ifeq ($(strip $(MK_DIR)),)
$(error vars.mk not yet included)
endif

ifeq ($(strip $(TARGET)),)
$(error No TARGET defined)
endif

TARGETLIB = lib$(TARGET).a

# Listing of phony targets.
.PHONY: clean cleanall all this

# default target:
all::
	@$(foreach REQ,$(LIBS_REQUIRED_BUILD_ORDER),$(MAKE) -C "$(dir $(REQ))" this; )
all:: this

cleanall:: clean
	@$(foreach REQ,$(LIBS_REQUIRED_BUILD_ORDER),echo "+++++ Cleaning $(notdir $(REQ))"; $(MAKE) -C "$(dir $(REQ))" clean; )

this:: lib

.PHONY: lib _lib_announce
_lib_announce:
	@echo
	@echo "-------- checking $(TARGETLIB) --------"
lib: _lib_announce $(TARGETLIB)
	-@$(call IDENT,$(TARGETLIB))
	@echo "-------- end $(TARGETLIB) --------"
	@echo

include $(MK_DIR)/compilationid.mk
include $(MK_DIR)/cso.mk

# various checks and listings on defined and unresolved externals
-include libsrequired.mk
LDFLAGS += $(THUMB) -nostartfiles
LDFLAGS += $(foreach SYM,$(REQUIRE_SYMBOLS),-u $(SYM))
include $(MK_DIR)/checkusedsymbols.mk

ifeq (,$(strip $(CHECK_FORBIDDEN_LIBC)))
$(error no CHECK_FORBIDDEN_LIBC defined by checkusedsymbols.mk)
endif

# Link: create ELF output file from object files.

$(TARGETLIB): $(ALLOBJ) $(LIBS_REQUIRED_BUILD_ORDER) $(MAKEFILE_LIST)
ifneq (*,$(UNRESOLVEDS_ALLOWED))
	@echo "Checking static library $@:"
	@$(call CHECK_FORBIDDEN_LIBC,$(LINK_TIME_OBJECTS) $(ALLOBJ),$(LIBS_REQUIRED_LINK_ORDER) $(UNRESOLVEDS_ALLOWED))
endif
	@echo "Creating static library $@:"
	$(RM) $@
	@$(MAKE) LINK_TIME_OBJECTS
	$(AR) $(ARFLAGS) $@ $(LINK_TIME_OBJECTS) $(ALLOBJ)
	@echo

.PHONY: list-defines
list-defines: $(ALLOBJ)
	@echo "-------- defined symbols in $(TARGETLIB) --------"
	@$(NM) -g $(ALLOBJ) | sed -n -e 's=^[0-9a-fA-F]*[ 	][ 	]*T[ 	]*==p' | sort -b
	@echo "-------- end of defined symbols in $(TARGETLIB) --------"
	@echo

.PHONY: list-needslibs
list-needslibs: $(ALLOBJ) $(LIBS_REQUIRED_BUILD_ORDER) 
	@echo "-------- libraries needed (and why) by $(TARGETLIB) --------"
	@$(CC) $(THUMB) -nostdlib $(ALLOBJ) -Wl,-r -o $(TARGETLIB)-reloc.log
	@$(NM) -u $(TARGETLIB)-reloc.log | sed -e 's/^[ 	]*U[ 	]*//' | sort -bu > $(TARGETLIB)-needs.log
	@$(RM) $(TARGETLIB)-reloc.log
	@( :; $(foreach SYM,$(UNRESOLVEDS_ALLOWED),echo "$(SYM) [allowed]"; ) $(foreach LIB,$(LIBS_REQUIRED_LINK_ORDER),$(NM) -g -o $(LIB) \
	| sed -n -e 's=^$(LIB):\([^:]*\):[0-9a-fA-F]*[ 	][ 	]*T[ 	]*\(.*\)=\2 $(notdir $(LIB)):=p'; ) ) \
	| sort -b | join -o2.2 -o0 $(TARGETLIB)-needs.log - \
	| sort -bu | sed -e '/\[allowed]/d'
	@$(RM) $(TARGETLIB)-needs.log
	@echo "-------- end of libraries needed by $(TARGETLIB) --------"
	@echo

# Target: clean project.
clean::
	$(RM) $(TARGETLIB) $(TARGETLIB)-*.log
