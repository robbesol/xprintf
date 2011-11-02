# Hey Emacs, this is a -*- makefile -*-
#

ifeq (,$(strip $(PROJECT_TOP_DIR)))
$(error PROJECT_TOP_DIR not set)
endif

_GITID_INFO_URL := $(strip $(shell cd $(PROJECT_TOP_DIR) && git config --get remote.origin.url | sed -e 's|//[^@/]*@|//|'))
ifneq (,$(strip $(_GITID_INFO_URL)))
VCS_URL := $(_GITID_INFO_URL)
endif

_GITID_VERSION_LONG_REMOTE := $(shell cd $(PROJECT_TOP_DIR) && git log -1 --remotes --format=format:'%H')
_GITID_VERSION_SHORT_REMOTE := $(shell cd $(PROJECT_TOP_DIR) && git log -1 --remotes --format=format:'%h')
_GITID_VERSION_LONG_LOCAL := $(shell cd $(PROJECT_TOP_DIR) && git log -1 --format=format:'%H')
_GITID_VERSION_SHORT_LOCAL := $(shell cd $(PROJECT_TOP_DIR) && git log -1 --format=format:'%h')

ifneq (,$(strip $(_GITID_VERSION_LONG_REMOTE)))
VCS_VERSION_LONG := $(_GITID_VERSION_LONG_REMOTE)
endif
ifneq (,$(strip $(_GITID_VERSION_SHORT_REMOTE)))
VCS_VERSION := $(_GITID_VERSION_SHORT_REMOTE)
endif

ifneq ($(strip $(_GITID_VERSION_LONG_LOCAL)),$(strip $(_GITID_VERSION_LONG_REMOTE)))
VCS_VERSION_LONG += + local $(_GITID_VERSION_LONG_LOCAL)
endif
ifneq ($(strip $(_GITID_VERSION_SHORT_LOCAL)),$(strip $(_GITID_VERSION_SHORT_REMOTE)))
VCS_VERSION += + local $(_GITID_VERSION_SHORT_LOCAL)
endif

VCS_VERSION_FULL := $(strip $(VCS_URL))
ifneq (,$(strip $(VCS_VERSION_LONG)))
VCS_VERSION_FULL += commit $(strip $(VCS_VERSION_LONG))
endif