# Hey Emacs, this is a -*- makefile -*-
#

ifeq (,$(strip $(TARGET)))
$(error No TARGET defined)
endif

ifeq (,$(strip $(PROJECT_TOP_DIR)))
PROJECT_TOP_DIR = ..
endif

_COMPILATIONID_MK_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

# The raw ident output triggers lots of errors being detected by Eclipse.
# Inserting a space in front of the first colon ("$Key: " -> "$Key : ") seems to prevent that.
# We remove the leading whitesoace just to be more pleaseng to the eye.
define IDENT
ident -q $1 | sed -n -e 's/: / : /' -e 's/^[ 	][ 	]*//p'
endef

# variables with two leading underscores are only used to build other variables 
__COMPILATIONID_HERE := $(shell pwd)
__COMPILATIONID_PROJPARENT_DIR := $(shell cd $(PROJECT_TOP_DIR)/.. && pwd)
__COMPILATIONID_PROJPARENT_REL_HERE := $(patsubst $(__COMPILATIONID_PROJPARENT_DIR)/%,%,$(__COMPILATIONID_HERE))

_COMPILATIONID_PROJ_BASENAME := $(firstword $(subst /, ,$(__COMPILATIONID_PROJPARENT_REL_HERE)))
_COMPILATIONID_PROJ_ROOT_DIR := $(__COMPILATIONID_PROJPARENT_DIR)/$(_COMPILATIONID_PROJ_BASENAME)

_COMPILATIONID_PROJ_REL_HERE := $(patsubst $(_COMPILATIONID_PROJ_BASENAME)/%,%,$(__COMPILATIONID_PROJPARENT_REL_HERE))

_SVNID_INFO_URL := $(strip $(shell cd $(_COMPILATIONID_PROJ_ROOT_DIR) && svn info | sed -n -e 's|^URL: svn[+]ssh|URL: svn|' -e 's|://[^@]*@|://|' -e 's/^URL://p'))
ifeq (,$(strip $(_SVNID_INFO_URL)))
_SVNID_INFO_URL := $(_COMPILATIONID_PROJ_BASENAME)/$(_COMPILATIONID_PROJ_REL_HERE)
endif

# use "svnversion -c" and strip, if mixed version, the leading (low) version number and colon
_SVNID_INFO_VERSION := $(shell cd $(_COMPILATIONID_PROJ_ROOT_DIR) && svnversion -c | sed -e 's|^[0-9]*:||')
ifeq (exported,$(strip $(_SVNID_INFO_VERSION)))
_SVNID_INFO_VERSION := noversion
endif
ifeq (,$(strip $(_SVNID_INFO_VERSION)))
_SVNID_INFO_VERSION := noversion
endif

_COMPILATIONID_STRING_LEAD := $$Build: $(_SVNID_INFO_URL) $(_SVNID_INFO_VERSION) $(_COMPILATIONID_PROJ_REL_HERE)/ compiled

_COMPILATIONID_FILES_BASENAME = $(GENERATED_SRC_DIR)compiledstring

_COMPILATIONID_TARGET_C = $(subst -,_,$(TARGET))
_COMPILATIONID_STRING_H_TOKEN = COMPILED_STRING_$(_COMPILATIONID_TARGET_C)_H_

COMPILATIONID_COMPILEDSTRING_VARIABLE = CompiledString_$(_COMPILATIONID_TARGET_C)
_COMPILATIONID_COMPILEDSTRING_DECL = const char $(COMPILATIONID_COMPILEDSTRING_VARIABLE)[]

# for the SoftwareVersion variable names, treat target XXX-dbg as target XXX
COMPILATIONID_SOFTWAREVERSION_VARIABLE = SoftwareVersion_$(_COMPILATIONID_TARGET_C)
_COMPILATIONID_VERSIONSTRING_DECL = const char $(COMPILATIONID_SOFTWAREVERSION_VARIABLE)[]

ifeq (,$(shell test -f $(_COMPILATIONID_FILES_BASENAME).c && grep '$(_COMPILATIONID_STRING_LEAD)' $(_COMPILATIONID_FILES_BASENAME).c))
# different compilation info -- force recompilation of compilationid info file
.PHONY: $(_COMPILATIONID_FILES_BASENAME).c
endif
SRC += $(_COMPILATIONID_FILES_BASENAME).c

$(_COMPILATIONID_FILES_BASENAME).h: $(_COMPILATIONID_MK_FILE)
	@test -d $(GENERATED_SRC_DIR) || mkdir $(GENERATED_SRC_DIR)
	@echo
	@echo "Creating: $@"
	@echo '#ifndef $(_COMPILATIONID_STRING_H_TOKEN)' >$@
	@echo '#define $(_COMPILATIONID_STRING_H_TOKEN)' >>$@
	@echo 'extern $(_COMPILATIONID_VERSIONSTRING_DECL);' >>$@
	@echo 'extern $(_COMPILATIONID_COMPILEDSTRING_DECL);' >>$@
	@echo '#endif /*$(_COMPILATIONID_STRING_H_TOKEN)*/' >>$@

$(_COMPILATIONID_FILES_BASENAME).c: $(_COMPILATIONID_FILES_BASENAME).h $(_COMPILATIONID_MK_FILE)
	@echo
	@echo "Creating $@"
	@echo '#include "compiledstring.h"' > $@
	@echo '$(_COMPILATIONID_VERSIONSTRING_DECL) =  "$(_SVNID_INFO_VERSION)";' >> $@
	@echo '$(_COMPILATIONID_COMPILEDSTRING_DECL) = "$(_COMPILATIONID_STRING_LEAD) " __DATE__ " " __TIME__ " $(CC) " __VERSION__ " $$";' >> $@

.PHONY: clean cleanall
clean cleanall::
	$(RM) $(_COMPILATIONID_FILES_BASENAME).*
