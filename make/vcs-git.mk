# Hey Emacs, this is a -*- makefile -*-
#

ifeq (,$(strip $(PROJECT_TOP_DIR)))
$(error PROJECT_TOP_DIR not set)
endif

_GITID_INFO_URL := $(strip $(shell cd $(PROJECT_TOP_DIR) && git config --get remote.origin.url))
ifneq (,$(strip $(_GITID_INFO_URL)))
VCS_URL := $(_GITID_INFO_URL)
endif

_GITID_REMOTES_VERSION := $(shell cd $(PROJECT_TOP_DIR) && git log -1 --remotes --format=format:'%H')
_GITID_LOCAL_VERSION := $(shell cd $(PROJECT_TOP_DIR) && git log -1 --format=format:'%H')

ifneq (,$(strip $(_GITID_REMOTES_VERSION)))
VCS_VERSION := $(_GITID_REMOTES_VERSION)
endif

ifneq ($(strip $(_GITID_LOCAL_VERSION)),$(strip $(_GITID_REMOTES_VERSION)))
VCS_VERSION += + local $(_GITID_LOCAL_VERSION)
endif
