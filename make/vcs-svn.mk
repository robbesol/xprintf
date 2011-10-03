# Hey Emacs, this is a -*- makefile -*-
#

ifeq (,$(strip $(PROJECT_TOP_DIR)))
$(error PROJECT_TOP_DIR not set)
endif

_SVNID_INFO_URL := $(strip $(shell cd $(PROJECT_TOP_DIR) && svn info | sed -n -e 's|^URL: svn[+]ssh|URL: svn|' -e 's|://[^@]*@|://|' -e 's/^URL://p'))
ifneq (,$(strip $(_SVNID_INFO_URL)))
VCS_URL := $(_SVNID_INFO_URL)
endif

# use "svnversion -c" and strip, if mixed version, the leading (low) version number and colon
_SVNID_INFO_VERSION := $(shell cd $(PROJECT_TOP_DIR) && svnversion -c | sed -e 's|^[0-9]*:||')

# if not under SVN, it returns "exported"
ifeq (exported,$(strip $(_SVNID_INFO_VERSION)))
_SVNID_INFO_VERSION :=
endif

ifeq (,$(strip $(_SVNID_INFO_VERSION)))
VCS_VERSION := $(_SVNID_INFO_VERSION)
endif
