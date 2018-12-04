INCLUDES = -I.  -I$(top_srcdir) $(all_includes) $(EA_EXTRA_INC)

EXTRA_DIST = intro_local intro_oreilly

AM_CXXFLAGS   = -DULXR_HAVE_CONFIG_H

CLEANFILES = *.~* *.~~* *~

all: $(bin_PROGRAMS)

bin_PROGRAMS = meerkat_client introspect

meerkat_client_LDADD = -lstdc++ ../libulxmlrpcpp.la
meerkat_client_LDFLAGS = $(all_libraries) $(LIBPTHREAD) $(LIBEXPAT) $(LIBICONV) $(LIBSOCKET)  $(EA_EXTRA_LIB)  $(PTHREAD_CFLAGS)
meerkat_client_SOURCES = meerkat_client.cpp

introspect_LDADD = -lstdc++ ../libulxmlrpcpp.la
introspect_LDFLAGS = $(all_libraries) $(LIBPTHREAD) $(LIBEXPAT) $(LIBICONV) $(LIBSOCKET)
introspect_SOURCES = introspect.cpp

info:
	@echo iconv: $(LIBICONV)
	@echo pthread: $(LIBPTHREAD)
	@echo cflags: $(CFLAGS)
	@echo cxxflags: $(CXXFLAGS)
	@echo mk ldadd: $(meerkat_client_LDADD)
	@echo mk ldflags: $(meerkat_client_LDFLAGS)

