# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

DOXYGEN_INPUTS += ../include
DOXYGEN_INPUTS += ../xprintf
DOXYGEN_INPUTS += extra

DOXYGEN_IMAGE_PATH += extra

DOXYGEN_EXAMPLES_DIR := examples
INCLUDEDIRS += $(DOXYGEN_INPUTS)
# ../../libdebug/Nodebug/include ../../libdebug/include

########################################################
# Doxygen stuff

# This should not be here, but in a user (host) independent location.
DOXYGEN = /Applications/Doxygen.app/Contents/Resources/doxygen

# first, do everything involving MAKEFILE_LIST: it changes with each "include"
_DOXMK_MKFILE_PATH := $(lastword $(MAKEFILE_LIST))

# default "doc":
.PHONY: doc
doc::

# legacy target "this"
.PHONY: this
this:: doc

# general vars

BASE_PROJECT_ROOT_DIR_SLASH = ..

ifeq (,$(strip $(DOXYGEN_PROJECT_NAME)))
DOXYGEN_PROJECT_NAME := $(lastword $(subst /, ,$(abspath $(BASE_PROJECT_ROOT_DIR_SLASH))))
endif

doc::
	@echo
	@echo "===== start generating doc: $(DOXYGEN_PROJECT_NAME) for $(DOXYGEN_PROJECT_NUMBER)"

########## regular doxygen stuff for target

ifeq (,$(strip $(DOXYGEN_INPUTS)))
$(error No DOXYGEN_INPUTS defined)
endif


DOXYGEN ?= doxygen

DOXYGEN_OUTPUT_DIRECTORY ?= .

DOXYGEN_CONFIG = Doxyfile

# find example sources
DOXYGEN_EXAMPLE_SRC := $(wildcard $(DOXYGEN_EXAMPLES_DIR)/*.c)
# their .XXX files
DOXYGEN_EXAMPLE_XXX =$(DOXYGEN_EXAMPLE_SRC:.c=.XXX)

DOXYGEN_EXAMPLES_CFLAGS ?= -Wno-unused -Wno-nested-externs -Wno-missing-declarations -Wno-missing-prototypes

#------------------

_DOC_MAKEFILE = $(lastword $(MAKEFILE_LIST))

ifneq (,$(DOXYGEN_EXAMPLES_DIR))

ifeq (,$(strip $(MK_DIR)))
include ../make/vars.mk
endif

%.XXX: %.c
	$(CC) -S $(CPPFLAGS) $(CFLAGS) $(DOXYGEN_EXAMPLES_CFLAGS) $< -o $@
	-@$(RM) $@

doc clean cleanall::
	-@$(RM) -f $(DOXYGEN_EXAMPLES_DIR)/*.XXX

endif

# versioning
PROJECT_TOP_DIR = ..
include $(dir $(_DOC_MAKEFILE))vcs.mk
# sets VCS_URL and VCS_VERSION
DOXYGEN_PROJECT_NUMBER = $(firstword $(strip $(VCS_VERSION)))

Doxygen/incl-inputs: $(_DOC_MAKEFILE) $(_DOXMK_MKFILE_PATH)
	@echo "Creating $@ for $(DOXYGEN_PROJECT_NAME)"
	@test -d Doxygen || mkdir Doxygen
	@echo >$@ "# generated `date`"
	@echo >>$@ "INPUT += Doxygen"
	@echo >>$@ "# DOXYGEN_INPUTS: [$(DOXYGEN_INPUTS)]"
	@$(foreach I,$(DOXYGEN_INPUTS),echo >>$@ "INPUT += $(I)"; echo >>$@ "STRIP_FROM_PATH += $(I)"; echo >>$@ "STRIP_FROM_INC_PATH += $(I)";)
	@echo >>$@ "HTML_FOOTER = extra/doxyfooter.htmlfrag"
	@echo >>$@ "INCLUDE_PATH = $(INCLUDEDIRS)"

Doxygen/incl-general: $(_DOC_MAKEFILE) $(_DOXMK_MKFILE_PATH)
	@echo "Creating $@ for $(DOXYGEN_PROJECT_NAME)"
	@test -d Doxygen || mkdir Doxygen
	@echo >$@ "# generated `date`"
	@echo >>$@ "PROJECT_NAME = $(DOXYGEN_PROJECT_NAME)"
	@echo >>$@ "@INCLUDE = Doxygen/incl-version"
	@echo >>$@ "PROJECT_BRIEF = \"$(DOXYGEN_PROJECT_BRIEF)\""
	@echo >>$@ "GENERATE_TAGFILE = Doxygen/tags-$(DOXYGEN_PROJECT_NAME)"
	@echo >>$@ "OUTPUT_DIRECTORY = $(DOXYGEN_OUTPUT_DIRECTORY)"
	@echo >>$@ "EXAMPLE_PATH = $(DOXYGEN_EXAMPLES_DIR)"
	@echo >>$@ "ECLIPSE_DOC_ID = com.$(DOXYGEN_PROJECT_NAME)"
ifneq (,$(DOXYGEN_IMAGE_PATH))
	@echo >>$@ "IMAGE_PATH = $(DOXYGEN_IMAGE_PATH)"
endif

.PHONY: Doxygen/incl-version
Doxygen/incl-version:
	@echo >>$@ "PROJECT_NUMBER = \"$(DOXYGEN_PROJECT_NUMBER)\""
	@echo >>$@ "ALIASES += sourceversion=\"$(DOXYGEN_PROJECT_NUMBER)\""

.PHONY: doc

doc:: Doxygen/incl-inputs Doxygen/incl-general Doxygen/incl-version $(DOXYGEN_CONFIG) $(DOXYGEN_EXAMPLE_XXX)
	@echo "Running Doxygen for $(DOXYGEN_PROJECT_NAME)"
	-@$(RM) -rf $(DOXYGEN_OUTPUT_DIRECTORY)/html/*
	@$(DOXYGEN) $(DOXYGEN_CONFIG)

doc::
	@echo "===== done generating doc: $(DOXYGEN_PROJECT_NAME) for $(DOXYGEN_PROJECT_NUMBER)"

clean cleanall::
	$(RM) -rf Doxygen
