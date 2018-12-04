INCLUDES = -I $(top_srcdir) -I. $(all_includes) $(EA_EXTRA_INC)

METASOURCES = AUTO

AM_CXXFLAGS   = -DULXR_HAVE_CONFIG_H

lib_LTLIBRARIES = libulxmlrpcpp_contrib.la

libulxmlrpcpp_contrib_la_SOURCES = \
  ssl_connection.cpp \
	mprocess_rpc_server.cpp

libulxmlrpcpp_contrib_la_LIBADD         = ../libulxmlrpcpp.la

libulxmlrpcpp_contrib_la_LDFLAGS = -no-undefined \
 -version-info $(ULXMLRPCPP_LT_CURRENT):$(ULXMLRPCPP_LT_REVISION):$(ULXMLRPCPP_LT_AGE)

libulxmlrpcpp_contribincludedir = $(includedir)/ulxmlrpcpp/contrib

libulxmlrpcpp_contribinclude_HEADERS = \
  $(libulxmlrpcpp_contrib_la_SOURCES:.cpp=.h)
