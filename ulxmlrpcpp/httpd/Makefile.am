DEF_FILES = index.html logo-ulxmlrpcpp.png
EXTRA_DIST = $(DEF_FILES) js-rpc-test.html js-xmlrpc-all_scripts.js

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/$(PACKAGE)/
	$(mkinstalldirs) $(DESTDIR)$(datadir)/$(PACKAGE)/httpd
	for hf in $(DEF_FILES); do \
          file=`echo $$hf | sed 's,.*/,,'`; \
          $(INSTALL_DATA) $(srcdir)/$$file $(DESTDIR)$(datadir)/$(PACKAGE)/httpd/$$file; \
	done
	@echo ""
	@echo "***********************************************************"
	@echo "* httpd directory prepared. Make sure to have correct      "
	@echo "* permissions for $(DESTDIR)$(datadir).                              "
	@echo "***********************************************************"

uninstall-local:
	for hf in $(DEF_FILES); do \
          file=`echo $$hf | sed 's,.*/,,'`; \
          rm -f "$(DESTDIR)$(datadir)/$(PACKAGE)/httpd/$$file"; \
	done

