# Hey Emacs, this is a -*- makefile -*-
#
# if you want a .map file for the executable, use this command line:
#   $ make all NEED_MAP=true
#
# if you want .lst files for every source, use this command line:
#   $ make all NEED_LST=true
#

ifeq ($(strip $(MK_DIR)),)
$(error vars.mk not yet included)
endif

ifeq ($(strip $(OBJPREFIX)),)
$(error no variable OBJPREFIX)
endif

# Use this for code in THUMB mode:
_THUMB_INTERWORK = -mthumb-interwork
NOTHUMB = $(_THUMB_INTERWORK)
THUMB = -mthumb $(_THUMB_INTERWORK)
## Generate code for the 16-bit Thumb instruction set. The default is to use the 
## 32-bit ARM instruction set.

ifdef VECTOR_LOCATION
CPPFLAGS += -D$(VECTOR_LOCATION)
endif

#===== Compiler flags.

ifneq ($(strip $(CC_DEBUG_FLAGS)),)
CFLAGS += $(CC_DEBUG_FLAGS)
ASFLAGS += $(CC_DEBUG_FLAGS)
endif

ifneq ($(strip $(CC_OPTIM_FLAGS)),)
CFLAGS += $(CC_OPTIM_FLAGS)
ASFLAGS += $(CC_OPTIM_FLAGS)
endif

include $(MK_DIR)/cflags.mk

ifneq ($(strip $(NEED_LST)),)
CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<) 
endif 

# Compiler flags to generate dependency files.
### DEPFLAGS += -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
DEPFLAGS += -MD
# Add "phony" targets (empty) for header files. Good for when a header file disappears (rename, obsolete). 
DEPFLAGS += -MP


#===== Assembler flags.

ASFLAGS += $(patsubst %,-I%,$(INCLUDEDIRS))
ifneq ($(strip $(NEED_LST)),)
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
ASFLAGS += -Wa,-adhlns=$(<:.S=.lst)
endif


############################################################################

SRCE = $(SRC:.c=.E) $(ASRC:.S=.E)
$(SRCE): %.E: %.c
	@echo
	@echo "Preprocessing C: $<"
	$(CPP) $(CPPFLAGS) $(CFLAGS) $< > $@
	
preprocessed: $(SRCE)


SRCASM = $(SRC:.c=.s)
$(SRCASM): %.s: %.c
	@echo
	@echo "Compiling C to assembly: $<"
	$(CC) -S $(THUMB) $(CPPFLAGS) $(CFLAGS) $< -o $@
	
SRCARMASM = $(SRCARM:.c=.s)
$(SRCARMASM): %.s: %.c
	@echo
	@echo "Compiling C to assembly (ARM-only): $<"
	$(CC) -S $(NOTHUMB) $(CPPFLAGS) $(CFLAGS) $< -o $@
	
asmcode: $(SRCASM) $(SRCARMASM)


# Define all object files.
COBJ = $(addprefix $(OBJPREFIX),$(SRC:.c=.o))
AOBJ = $(addprefix $(OBJPREFIX),$(ASRC:.S=.o))
COBJARM = $(addprefix $(OBJPREFIX),$(SRCARM:.c=.o))
AOBJARM = $(addprefix $(OBJPREFIX),$(ASRCARM:.S=.o))
# all of the above:
ALLOBJ = $(COBJ) $(AOBJ) $(COBJARM) $(AOBJARM)

ifeq ($(strip $(ALLOBJ)),)
define _NO_SOURCE_ERROR_TEXT
No sources defined. Use any of the following variables:
  SRC  for ISO-C (compile to 16-bit Thumb instruction set)
  ASRC  for asm (compile to 16-bit Thumb instruction set)
  SRCARM  for ISO-C (compile to 32-bit ARM instruction set)
  ASRCARM  for asm (compile to 32-bit ARM instruction set)
endef
$(error $(_NO_SOURCE_ERROR_TEXT))
else
.PRECIOUS : $(ALLOBJ)
endif

# Compile: create object files from C source files. ARM/Thumb
$(COBJ): $(OBJPREFIX)%.o: %.c
	@echo
	@echo "Compiling C: $<"
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(THUMB) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) $< -o $@

# Compile: create object files from C source files. ARM-only
$(COBJARM): $(OBJPREFIX)%.o: %.c
	@echo
	@echo "Compiling C (ARM-only): $<"
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(NOTHUMB) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) $< -o $@


# Assemble: create object files from assembler source files. ARM/Thumb
$(AOBJ): $(OBJPREFIX)%.o: %.S
	@echo
	@echo "Assembling: $<"
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(AS) -c $(THUMB) $(CPPFLAGS) $(ASFLAGS) $(DEPFLAGS) $< -o $@


# Assemble: create object files from assembler source files. ARM-only
$(AOBJARM): $(OBJPREFIX)%.o: %.S
	@echo
	@echo "Assembling (ARM-only): $<"
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(AS) -c $(NOTHUMB) $(CPPFLAGS) $(ASFLAGS) $(DEPFLAGS) $< -o $@

LINK_TIME_OBJECTS = $(addprefix $(OBJPREFIX),$(COMPILE_AT_LINK_TIME_SRC:.c=.o))

$(LINK_TIME_OBJECTS): $(OBJPREFIX)%.o: %.c
	@echo
	@echo "Compiling last-minute C: $<"
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(THUMB) $(CPPFLAGS) $(CFLAGS) $< -o $@
	
.PHONY:  LINK_TIME_OBJECTS
LINK_TIME_OBJECTS:: $(LINK_TIME_OBJECTS)
	$(if $(filter-out $?,$(LINK_TIME_OBJECTS)),$(error LINK_TIME_OBJECTS not compiled: $(filter-out $?,$(LINK_TIME_OBJECTS))))

LINK_TIME_OBJECTS::
	@# avoid message "Nothing to be done for `LINK_TIME_OBJECTS'"

# Include the dependency files, but not during a clean, %clean or clean%.
MAKE_CLEAN = $(sort $(filter %clean,$(MAKECMDGOALS)) $(filter clean%,$(MAKECMDGOALS)))
ifeq (,$(MAKE_CLEAN))
# no *clean* target specified
-include $(COBJ:.o=.d) $(COBJARM:.o=.d)
endif 

# Target: clean project.
.PHONY: clean cleanall
clean cleanall::
	-$(RM) -rf obj
	$(RM) $(SRCE)
	$(RM) $(SRCASM) $(SRCARMASM)


# doc target:
.PHONY: doc
ifeq (,$(realpath ../doc/Makefile))
doc:
	@echo >&2 "***** no doc/Makefile -> no documentation available for $(TARGET)"
else
doc:
	$(MAKE) -C ../doc doc
endif
