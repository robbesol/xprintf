# Hey Emacs, this is a -*- makefile -*-
#

VERSION_CONTROL_SYSTEM ?= git
#VERSION_CONTROL_SYSTEM ?= svn

ifeq (,$(strip $(PROJECT_TOP_DIR)))
$(error PROJECT_TOP_DIR not set)
endif

VCS_URL :=
VCS_VERSION :=

# first, do everything involving MAKEFILE_LIST: it changes with each "include"
_VCS_MKFILE_PATH := $(lastword $(MAKEFILE_LIST))

_VCS_X_MK_FILE = $(dir $(_VCS_MKFILE_PATH))vcs-$(VERSION_CONTROL_SYSTEM).mk
ifeq (,$(wildcard $(_VCS_X_MK_FILE)))
$(warning No makefile file VCS $(VERSION_CONTROL_SYSTEM): expected $(_VCS_X_MK_FILE))
endif

-include $(_VCS_X_MK_FILE)