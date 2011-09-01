# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

.PHONY: all this
all this:
	$(MAKE) -C build $@

.PHONY: test
test:
	$(MAKE) -C test $@

.PHONY: doc
doc:
	$(MAKE) -C doc $@