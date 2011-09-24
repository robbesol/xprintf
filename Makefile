# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

.PHONY: all this
all this:
	$(MAKE) -C build $@

.PHONY: clean
clean:
	$(foreach D,$(dir $(wildcard */Makefile)),$(MAKE) -C $D $@;)
	
.PHONY: test
test:
	$(MAKE) -C test $@

.PHONY: doc
doc:
	$(MAKE) -C doc $@

.PHONY: doc-webinstall
doc-webinstall: doc
	@test -d doc/html || { echo >&2 "***** No doc/ ??"; exit 1; }
	scp -r doc/html shell.berlios.de:/home/groups/xprintf/htdocs/doc 