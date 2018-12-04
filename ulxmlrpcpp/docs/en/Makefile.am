SUBDIRS = . img common images

.PHONY: doc doc-dist remote-install pdf-files

#--------------------------------------------------------

remote-install: remote-install-db

doc: doc-db pdf-files

doc-dist: doc-dist-db

#--------------------------------------------------------

remote-install-st: all
	-scp -v *.html                        ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/hb
	-scp -v img/*.gif                     ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/hb

remote-install-db: all
	-scp -v var/*.html                    ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/handbook
	-scp $(srcdir)/common/*.css           ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/handbook/common
	-scp $(srcdir)/images/*.png           ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/handbook/images
	-scp $(srcdir)/images/callouts/*.png  ewald-arnold@ulxmlrpcpp.sourceforge.net:/home/groups/u/ul/ulxmlrpcpp/htdocs/handbook/images/callouts

#--------------------------------------------------------

doc-st:
	test $(srcdir)/index.nif -ef index.nif ||  cp $(srcdir)/index.nif .
	sgml2html --split=2  --imagebuttons --nice  $(srcdir)/index.sgml

#--------------------------------------------------------

html-chunk-files:
	mkdir -pv var/common
	mkdir -pv var/images/callouts
	cp $(srcdir)/common/*.css var/common
	cp $(srcdir)/images/*.png var/images
	cp $(srcdir)/images/callouts/*.png var/images/callouts
	$(XSLTPROC) -o var/  $(srcdir)/customize-chunked.xsl  $(srcdir)/index.docbook

html-flat-files:
	mkdir -pv var/common
	mkdir -pv var/images/callouts
	cp $(srcdir)/common/*.css var/common
	cp $(srcdir)/images/*.png var/images
	cp $(srcdir)/images/callouts/*.png var/images/callouts
	$(XSLTPROC) -o var/  $(srcdir)/customize-flat.xsl  $(srcdir)/index.docbook

doc-db: html-chunk-files

#--------------------------------------------------------

pdf-files:
	mkdir -pv var/common
	mkdir -pv var/images/callouts
	cp $(srcdir)/common/*.css var/common
	cp $(srcdir)/images/*.png var/images
	cp $(srcdir)/images/callouts/*.png var/images/callouts
	$(XSLTPROC) --nonet --output var/ulxmlrpcpp.fo $(srcdir)/customize-pdf.xsl $(srcdir)/index.docbook
	$(FOP)  var/ulxmlrpcpp.fo var/ulxmlrpcpp.pdf
	cp var/ulxmlrpcpp.pdf $(top_builddir)/$(PACKAGE)-$(VERSION)-doc.pdf

#--------------------------------------------------------

doc-dist-st: doc-st
	-test -d $(PACKAGE)-$(VERSION)-doc && rm -rf $(PACKAGE)-$(VERSION)-doc
	mkdir $(PACKAGE)-$(VERSION)-doc
	cp -R *.html $(PACKAGE)-$(VERSION)-doc
	cp -R $(srcdir)/img/*.gif $(PACKAGE)-$(VERSION)-doc
	tar cfj $(PACKAGE)-$(VERSION)-doc.tar.bz2 $(PACKAGE)-$(VERSION)-doc/*
	zip  -r $(PACKAGE)-$(VERSION)-doc.zip  $(PACKAGE)-$(VERSION)-doc/*
	mv $(PACKAGE)-$(VERSION)-doc.tar.bz2 $(top_builddir)
	mv $(PACKAGE)-$(VERSION)-doc.zip $(top_builddir)
	rm -rf $(PACKAGE)-$(VERSION)-doc

doc-dist-db: doc-db
	-test -d $(PACKAGE)-$(VERSION)-doc && rm -rf $(PACKAGE)-$(VERSION)-doc
	mkdir -p $(PACKAGE)-$(VERSION)-doc
	cp -R var/* $(PACKAGE)-$(VERSION)-doc
	tar cfj $(PACKAGE)-$(VERSION)-doc.tar.bz2 $(PACKAGE)-$(VERSION)-doc/*
	zip  -r $(PACKAGE)-$(VERSION)-doc.zip  $(PACKAGE)-$(VERSION)-doc/*
	mv $(PACKAGE)-$(VERSION)-doc.tar.bz2 $(top_builddir)
	mv $(PACKAGE)-$(VERSION)-doc.zip $(top_builddir)
	rm -rf $(PACKAGE)-$(VERSION)-doc

CLEANFILES = *.~* *.~~* *~ 	var/* var/common/* var/images/* var/images/callouts/*

EXTRA_DIST = index.sgml index.nif \
             appendix.docbook sect_unicode.docbook \
             sect_generator.docbook \
             index.docbook index.docbook bookinfo.docbook faq.docbook credits.docbook \
             sect_variable_types.docbook sect_basic_security.docbook sect_cookie.docbook \
             sect_http_client_server.docbook sect_mt_http_client_server.docbook \
             sect_mt_xmlrpc_client_server.docbook sect_proxy.docbook sect_ssl.docbook \
             sect_tcp_conn.docbook \
             sect_wbxml_parser.docbook sect_xml_parser.docbook sect_xmlrpc_client_server.docbook \
             sect_transfer.docbook sect_intro.docbook \
             customize-flat.xsl customize-chunked.xsl customize.xsl customize-pdf.xsl

#################################################################

htmldir = $(pkgdatadir)/html

install-data-hook: doc
	$(mkdir_p) $(htmldir)/handbook/common
	$(mkdir_p) $(htmldir)/handbook/images/callouts
	$(INSTALL) var/*.pdf $(pkgdatadir)
	$(INSTALL) var/*.html                      $(htmldir)/handbook
	$(INSTALL) $(srcdir)/common/*.css          $(htmldir)/handbook/common
	$(INSTALL) $(srcdir)/images/*.png          $(htmldir)/handbook/images
	$(INSTALL) $(srcdir)/images/callouts/*.png $(htmldir)/handbook/images/callouts

# Automake's "distcheck" is sensitive to having files left over
# after "make uninstall", so we have to clean up the install hook.
uninstall-local:
	rm -rf $(htmldir)/handbook

