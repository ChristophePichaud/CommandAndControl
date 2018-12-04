EXTRA_DIST = # $(PACKAGE).pot $(POTEMPLATES) $(GMOFILES)

GMOFILES      = de.gmo
POTEMPLATES   = $(GMOFILES:.gmo=.po)

CLEANFILES = *.~* *.~~* *~

merge:
	@catalogs='$(POTEMPLATES)'; \
	for cat in $$catalogs; do \
	name=$(PACKAGE).pot ; \
	echo $$cat $$name; \
	msgmerge $$cat $$name > $$cat.new ; \
	if diff $$cat $$cat.new; then \
		rm $$cat.new;  \
	else  \
		mv $$cat.new $$cat ; \
	fi; \
	done

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	file=$(srcdir)/`echo $* | sed 's,.po/,,'`.gmo \
        && rm -f $$file && $(GMSGFMT) -o $$file $<

localedir = $(kde_locale)

xx-install-yes: all
	@catalogs='$(GMO_INSTALL)'; \
	for cat in $$catalogs; do \
	destdir=$(localedir); \
	lang=`echo $$cat | sed 's/\.kde[12]\.gmo//'`; \
	name=$(PACKAGE).mo \
	dir=$$destdir/$$lang/LC_MESSAGES; \
	$(mkinstalldirs) $$dir; \
	$(INSTALL_DATA) $$cat $$dir/$$name; \
	  echo "installing $$cat as $$dir/$$name"; \
	done

xx-uninstall-local:
	@catalogs='$(GMO_INSTALL)'; \
	for cat in $$catalogs; do \
	destdir=$(localedir); \
	lang=`echo $$cat | sed 's/\.gmo//'`; \
	name=$(PACKAGE).mo; \
	dir=$$destdir/$$lang/LC_MESSAGES; \
	rm -f $$cat $$dir/$$name; \
	echo "removing $$dir/$$name" ; \
	done
