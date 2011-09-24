# Hey Emacs, this is a -*- makefile -*-
#
# if you want a .map file for the executable, use this command line:
#   $ make all NEED_MAP=true
#

ifeq (,$(strip $(MK_DIR)))
$(error vars.mk not yet included)
endif

ifeq (,$(strip $(OBJPREFIX)))
$(error no variable OBJPREFIX)
endif

#===== Compiler flags.

ifneq (,$(strip $(CC_DEBUG_FLAGS)))
CFLAGS += $(CC_DEBUG_FLAGS)
endif

ifneq (,$(strip $(CC_OPTIM_FLAGS)))
CFLAGS += $(CC_OPTIM_FLAGS)
endif

############################################################################

SRCE = $(SRC:.c=.E)
$(SRCE): $(MAKEFILE_LIST)
$(SRCE): %.E: %.c
	@echo
	@echo "Preprocessing C: $<"
	$(CPP) $(CPPFLAGS) $(CFLAGS) $< > $@

preprocessed: $(SRCE)


SRCASM = $(SRC:.c=.s)
$(SRCASM): $(MAKEFILE_LIST)
$(SRCASM): %.s: %.c
	@echo
	@echo "Compiling C to assembly: $<"
	$(CC) -S $(CPPFLAGS) $(CFLAGS) $< -o $@

asmcode: $(SRCASM)


# Define all object files.
ALLOBJ = $(addprefix $(OBJPREFIX),$(SRC:.c=.o)) 
.PRECIOUS : $(ALLOBJ)
$(ALLOBJ): $(MAKEFILE_LIST)

$(OBJPREFIX)%.o: %.c
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) $< -o $@


# Include the dependency files, but not during a clean, %clean or clean%.
MAKE_CLEAN = $(sort $(filter %clean,$(MAKECMDGOALS)) $(filter clean%,$(MAKECMDGOALS)))
ifeq (,$(MAKE_CLEAN))
# no *clean* target specified
-include $(ALLOBJ:.o=.d)
endif 


# Target: clean project.
.PHONY: clean cleanall
clean cleanall::
	-$(RM) -rf obj
	$(RM) $(SRCE)
	$(RM) $(SRCASM)
