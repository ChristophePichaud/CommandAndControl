all:

doc: apidoc

CLEAN_FILES = html/*

.PHONY: api-index apidoc apidoc-dist remote-install

remote-install: doc
	-scp -v html/*.html  ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/api
	-scp -v html/*.png   ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/api
	-scp -v html/*.gif   ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/api
	-scp -v html/*.css   ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/api

apidoc:
	-test -d html && rm -rf html
	doxygen $(top_builddir)/ulxmlrpcpp.doxygen
	-test -f $(PNGCRUSH) &&  $(PNGCRUSH) -d new-png html/*.png && cp new-png/*.png html/ && rm -r new-png

api-index:
	cd $(srcdir)/html && doxytag -s search.idx

apidoc-dist: apidoc
	-test -d $(PACKAGE)-$(VERSION)-apidoc && rm -rf $(PACKAGE)-$(VERSION)-apidoc
	mkdir $(PACKAGE)-$(VERSION)-apidoc
	cp -R html/* $(PACKAGE)-$(VERSION)-apidoc
	-test -f ulxmlrpcpp.tag && cp ulxmlrpcpp.tag  $(PACKAGE)-$(VERSION)-apidoc
	tar cfj $(PACKAGE)-$(VERSION)-apidoc.tar.bz2 $(PACKAGE)-$(VERSION)-apidoc/*
	zip  -r $(PACKAGE)-$(VERSION)-apidoc.zip  $(PACKAGE)-$(VERSION)-apidoc/*
	mv $(PACKAGE)-$(VERSION)-apidoc.tar.bz2 $(top_builddir)
	mv $(PACKAGE)-$(VERSION)-apidoc.zip $(top_builddir)
	rm -rf $(PACKAGE)-$(VERSION)-apidoc

#################################################################

htmldir = $(pkgdatadir)/html

CLEANFILES = *.~* *.~~* *~ 	./html/*

install-data-hook: doc
	$(mkdir_p) $(htmldir)/api-doc
	rm -rf html/search
	$(INSTALL) html/* $(htmldir)/api-doc

# Automake's "distcheck" is sensitive to having files left over
# after "make uninstall", so we have to clean up the install hook.
uninstall-local:
	rm -rf $(htmldir)/api-doc



