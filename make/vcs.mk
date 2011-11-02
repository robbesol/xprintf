# Hey Emacs, this is a -*- makefile -*-
#

VERSION_CONTROL_SYSTEM ?= git
#VERSION_CONTROL_SYSTEM ?= svn

ifeq (,$(strip $(PROJECT_TOP_DIR)))
$(error PROJECT_TOP_DIR not set)
endif

# If you use a VCS method XXX we expeect a makefile vcs-XXX.mk in the directory of the makefile you are now reading. 
# That makefile should set the following variables: 
# VCS_URL -- the URL of the source repository (no default)
# VCS_VERSION == the "normal" version string (no default)
# VCS_VERSION_LONG -- the "long" version string (default: VCS_VERSION)
# VCS_VERSION_FULL -- the full version string, combining the URL and version (default: VCS_URL VCS_VERSION_LONG)

VCS_URL :=
VCS_VERSION :=
VCS_VERSION_LONG :=
VCS_VERSION_FULL :=

# first, do everything involving MAKEFILE_LIST: it changes with each "include"
_VCS_MKFILE_PATH := $(lastword $(MAKEFILE_LIST))

_VCS_X_MK_FILE = $(dir $(_VCS_MKFILE_PATH))vcs-$(VERSION_CONTROL_SYSTEM).mk
ifeq (,$(wildcard $(_VCS_X_MK_FILE)))
$(warning No makefile file VCS $(VERSION_CONTROL_SYSTEM): expected $(_VCS_X_MK_FILE))
endif

-include $(_VCS_X_MK_FILE)

ifeq (,$(strip $(VCS_VERSION_LONG)))
VCS_VERSION_LONG := $(strip $(VCS_VERSION))
endif
ifeq (,$(strip $(VCS_VERSION_FULL)))
VCS_VERSION_FULL := $(strip $(VCS_URL) $(VCS_VERSION_LONG))
endif