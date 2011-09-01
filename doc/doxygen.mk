# Hey Emacs, this is a -*- makefile -*-
#
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

ifeq ($(strip $(DOXYGEN_PROJECT_NAME)),)
DOXYGEN_PROJECT_NAME := $(lastword $(subst /, ,$(abspath ..)))
endif

doc::
	@echo "===== start generating doc: $(DOXYGEN_PROJECT_NAME)"

########## regular doxygen stuff for target

ifeq ($(strip $(DOXYGEN_INPUTS)),)
$(error No DOXYGEN_INPUTS defined)
endif

DOXYGEN ?= doxygen

DOXYGEN_OUTPUT_DIRECTORY ?= .

DOXYGEN_CONFIG = Doxyfile

ifneq (,$(DOXYGEN_EXAMPLES_DIR))

# find example sources
DOXYGEN_EXAMPLE_SRC := $(wildcard $(DOXYGEN_EXAMPLES_DIR)/*.c)
# their .XXX files
DOXYGEN_EXAMPLE_XXX =$(DOXYGEN_EXAMPLE_SRC:.c=.XXX)

DOXYGEN_EXAMPLES_CFLAGS ?= -Wno-unused -Wno-nested-externs -Wno-missing-declarations -Wno-missing-prototypes

include ../build/cflags.mk

%.XXX: %.c
	$(CC) -S $(CPPFLAGS) $(CFLAGS) $(DOXYGEN_EXAMPLES_CFLAGS) $< -o $@
	-@$(RM) $@

doc clean cleanall::
	-@$(RM) -f $(DOXYGEN_EXAMPLES_DIR)/*.XXX

endif
	
Doxygen/incl-inputs: Makefile $(_DOXMK_MKFILE_PATH)
	@echo "Creating $@ for $(DOXYGEN_PROJECT_NAME)"
	@test -d Doxygen || mkdir Doxygen
	@echo >$@ "# generated `date`"
	@echo >>$@ "INPUT += Doxygen"
	@$(foreach I,$(DOXYGEN_INPUTS),echo >>$@ "INPUT += $(I)"; echo >>$@ "STRIP_FROM_PATH += $(I)"; echo >>$@ "STRIP_FROM_INC_PATH += $(I)";)
	@echo >>$@ "HTML_FOOTER = doxyfooter.htmlfrag"
	@echo >>$@ "INCLUDE_PATH = $(INCLUDEDIRS)"

Doxygen/incl-general: Makefile $(_DOXMK_MKFILE_PATH)
	@echo "Creating $@ for $(DOXYGEN_PROJECT_NAME)"
	@test -d Doxygen || mkdir Doxygen
	@echo >$@ "# generated `date`"
	@echo >>$@ "PROJECT_NAME = $(DOXYGEN_PROJECT_NAME)"
	@echo >>$@ "PROJECT_NUMBER = \"$(DOXYGEN_PROJECT_NUMBER)\""
	@echo >>$@ "ALIASES += sourceversion=\"$(DOXYGEN_PROJECT_NUMBER)\""
	@echo >>$@ "PROJECT_BRIEF = \"$(DOXYGEN_PROJECT_BRIEF)\""
	@echo >>$@ "GENERATE_TAGFILE = Doxygen/tags-$(DOXYGEN_PROJECT_NAME)"
	@echo >>$@ "OUTPUT_DIRECTORY = $(DOXYGEN_OUTPUT_DIRECTORY)"
	@echo >>$@ "EXAMPLE_PATH = $(DOXYGEN_EXAMPLES_DIR)"
	@echo >>$@ "ECLIPSE_DOC_ID = com.$(DOXYGEN_PROJECT_NAME)"
ifneq (,$(DOXYGEN_IMAGE_PATH))
	@echo >>$@ "IMAGE_PATH = $(DOXYGEN_IMAGE_PATH)"
endif

.PHONY: doc

doc:: Doxygen/incl-inputs Doxygen/incl-general $(DOXYGEN_CONFIG) $(DOXYGEN_EXAMPLE_XXX)
	@echo "Running Doxygen for $(DOXYGEN_PROJECT_NAME)"
	-@$(RM) -rf $(DOXYGEN_OUTPUT_DIRECTORY)/html/*
	@$(DOXYGEN) $(DOXYGEN_CONFIG)

doc::
	@echo "===== done generating doc: $(DOXYGEN_PROJECT_NAME)"

clean cleanall::
	$(RM) -rf Doxygen

# versioning
_SVN_VERSION = $(shell cd $(BASE_PROJECT_ROOT_DIR_SLASH) && svnversion)
ifeq (exported,$(strip $(_SVN_VERSION)))
# directories without SVN info are reported as "exported"
_SVN_VERSION =
endif
ifeq (,$(strip $(_SVN_VERSION)))
DOXYGEN_PROJECT_NUMBER = (not versioned)
else
DOXYGEN_PROJECT_NUMBER = release $(patsubst %M,%,$(patsubst %P,%,$(strip $(_SVN_VERSION))))
endif
	