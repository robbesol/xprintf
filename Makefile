# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

DOC_INSTALL_SRCDIR = doc/html
DOC_INSTALL_DESTDIR = ../xprintf-web/doc

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
doc-webinstall: $(dir $(DOC_INSTALL_DESTDIR)) doc
	@test -d $(DOC_INSTALL_SRCDIR) || { echo >&2 "***** No doc/ ??"; exit 1; }
	$(RM) -rf $(DOC_INSTALL_DESTDIR)
	#mkdir $(DOC_INSTALL_DESTDIR)
	cp -R $(DOC_INSTALL_SRCDIR) $(DOC_INSTALL_DESTDIR)

$(dir $(DOC_INSTALL_DESTDIR)):
	@echo >&2 "***** doc installation directory not found: $(dir $(DOC_INSTALL_DESTDIR))"
	@echo >&2 "***** check out project first"
	@exit 1
	