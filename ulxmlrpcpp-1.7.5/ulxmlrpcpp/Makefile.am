INCLUDES = -I. -I $(top_srcdir)  $(all_includes) $(EA_EXTRA_INC)

SUBDIRS = . contrib docs tests po apps httpd

AM_CXXFLAGS = -DULXR_DATADIR=\"$(datadir)\" -DULXR_HAVE_CONFIG_H

lib_LTLIBRARIES          = libulxmlrpcpp.la
libulxmlrpcpp_la_LDFLAGS = -no-undefined \
 -version-info $(ULXMLRPCPP_LT_CURRENT):$(ULXMLRPCPP_LT_REVISION):$(ULXMLRPCPP_LT_AGE)

# -release $(ULXMLRPCPP_LT_RELEASE)

libulxmlrpcpp_la_LIBADD =  \
	$(LIBPTHREAD) $(LIBEXPAT) $(LIBICONV) $(LIBSOCKET) $(EA_EXTRA_LIB)  $(PTHREAD_CFLAGS) \
	$(LIBSSL)

#ulxr_auth.cpp
libulxmlrpcpp_la_SOURCES = ulxmlrpcpp.cpp ulxr_cached_resource.cpp \
	ulxr_call.cpp ulxr_callparse.cpp ulxr_callparse_base.cpp ulxr_callparse_wb.cpp \
	ulxr_connection.cpp ulxr_dispatcher.cpp ulxr_except.cpp ulxr_expatwrap.cpp \
	ulxr_file_resource.cpp ulxr_htmlform_handler.cpp ulxr_http_client.cpp ulxr_http_protocol.cpp \
	ulxr_http_server.cpp ulxr_log4j.cpp ulxr_mtrpc_server.cpp ulxr_mutex.cpp ulxr_protocol.cpp \
	ulxr_requester.cpp ulxr_response.cpp ulxr_responseparse.cpp ulxr_responseparse_base.cpp \
	ulxr_responseparse_wb.cpp ulxr_signature.cpp ulxr_ssl_connection.cpp ulxr_tcpip_connection.cpp \
	ulxr_value.cpp ulxr_valueparse.cpp ulxr_valueparse_base.cpp ulxr_valueparse_wb.cpp \
	ulxr_virtual_resource.cpp ulxr_wbxmlparse.cpp ulxr_xmlparse.cpp ulxr_xmlparse_base.cpp

EXTRA_DIST = \
  ulxr_config-bcb5.h \
  ulxr_config-cygwin.h \
  ulxr_config-msvc.h

.PHONY: ulxr_config-bcb5.h ulxr_config-msvc.h ulxr_config-cygwin.h ulxr_config.h

pkginclude_HEADERS = \
  $(libulxmlrpcpp_la_SOURCES:.cpp=.h) \
  ulxr_method_adder.h \
  ulxr_config.h

CLEANFILES = *.~* *.~~* *~

ulxr_config.h : ../config.h
	echo "/* Edit this file with care. It is automatically generated from config.h. */" >$@
	grep "#define[ ]\+VERSION[ ]\+" ../config.h | sed "s/#define[ ]\+VERSION[ ]\+\(.*\)/#define ULXMLRPCPP_VERSION \\1/g">>$@
	grep "#define[ ]\+PACKAGE[ ]\+" ../config.h | sed "s/#define[ ]\+PACKAGE[ ]\+\(.*\)/#define ULXR_PACKAGE \\1/g">>$@
	grep ULXR_UNICODE ../config.h >>$@
	-grep HAVE_ICONV ../config.h && (grep HAVE_ICONV ../config.h >>$@)
	grep ULXR_INCLUDE_SSL_STUFF ../config.h >>$@
	grep ULXR_MULTITHREADED ../config.h >>$@
	echo "/* Define to re-use a server socket (saves time while debugging). */" >>$@
	echo "/* #undef ULXR_REUSE_SOCKET */" >>$@
	echo "" >>$@
	echo "/* define if you want to omit tcp/ip and http stuff. */" >>$@
	echo "/* #undef ULXR_OMIT_TCP_STUFF */" >>$@
	echo "" >>$@
	echo "/* Version number of package as long int */" >>$@
	echo "#define ULXR_VERSION_NUMBER \"$(ULXR_VERSION_NUMBER)\"" >>$@
	echo "" >>$@
	-test $@ -ef $(srcdir)/$@ || cp -v $@ $(srcdir)


ulxr_config-cygwin.h:
	echo "// #define's for Cygwin" >$@
	echo "// derived from automatically generated config.h. See Makefile.am." >>$@
	echo "" >>$@
	echo "/* Define to turn debugging globally on. */" >>$@
	echo "#undef DEBUG" >>$@
	echo "" >>$@
	echo "/* Define to use log4j logging host. */" >>$@
	echo "#undef ULXR_LOG4J" >>$@
	echo "" >>$@
	echo "/* Define to compile for unicode environment. */" >>$@
	echo "#undef ULXR_UNICODE" >>$@
	echo "" >>$@
	echo "/* Define to compile only the unicode helper methods. */" >>$@
	echo "#undef ULXR_UNICODE_ONLY_HELPERS" >>$@
	echo "" >>$@
	echo "/* Define to compile for openssl support. */" >>$@
	echo "#undef ULXR_INCLUDE_SSL_STUFF" >>$@
	echo "" >>$@
	echo "/* Define if you have a multithreaded environment. */" >>$@
	echo "#define ULXR_MULTITHREADED" >>$@
	echo "" >>$@
	echo "/* Define if you have the <expat.h> header file. */" >>$@
	echo "#define HAVE_EXPAT_H 1" >>$@
	echo "" >>$@
	echo "/* Define if you have the <iconv.h> header file. */" >>$@
	echo "/* #undef HAVE_ICONV_H 1 */" >>$@
	echo "" >>$@
	echo "/* Define as const if the declaration of iconv() needs const. */" >>$@
	echo "#define ICONV_CONST" >>$@
	echo "" >>$@
	echo "/* Define if you have the <sys/socket.h> header file. */" >>$@
	echo "#define HAVE_SYS_SOCKET_H 1" >>$@
	echo "" >>$@
	echo "/* Define if you have the strcase function. */" >>$@
	echo "#undef HAVE_STRCASECMP" >>$@
	echo "" >>$@
	echo "/* root directory for the built-in http server */" >>$@
	echo "#if !defined(ULXR_DATADIR)" >>$@
	echo "# define ULXR_DATADIR \"c:\\\\ulxmlrpcpp\"" >>$@
	echo "#endif" >>$@
	echo "" >>$@
	echo "/* define if WinSocket already started somewhere else. */" >>$@
	echo "/* #undef ULXR_NO_WSA_STARTUP */" >>$@
	echo "" >>$@
	echo "/* define if you want to omit tcp/ip and http stuff. */" >>$@
	echo "/* #undef ULXR_OMIT_TCP_STUFF */" >>$@
	echo "" >>$@
	echo "/* define if you want to automatically create Value()s from int, bool, .. */" >>$@
	echo "/* #undef ULXR_USE_INTRINSIC_VALUE_TYPES */" >>$@
	echo "" >>$@
	echo "/* define if you want to create a Struct::Member like this: ("name" << value) */" >>$@
	echo "/* #undef ULXR_STRUCT_MEMBER_FROM_NAME_VALUE */" >>$@
	echo "" >>$@
	echo "/* debugging: show call trace. */" >>$@
	echo "/* #undef ULXR_SHOW_TRACE */" >>$@
	echo "" >>$@
	echo "/* debugging: show debug information. */" >>$@
	echo "/* #undef ULXR_DEBUG_OUTPUT */" >>$@
	echo "" >>$@
	echo "/* debugging: show transfered data. */" >>$@
	echo "/* #undef ULXR_SHOW_CONTENT */" >>$@
	echo "" >>$@
	echo "/* debugging: show http header contents. */" >>$@
	echo "/* #undef ULXR_SHOW_HTTP */" >>$@
	echo "" >>$@
	echo "/* debugging: show each read characters. */" >>$@
	echo "/* #undef ULXR_SHOW_READ */" >>$@
	echo "" >>$@
	echo "/* debugging: show each written characters. */" >>$@
	echo "/* #undef ULXR_SHOW_WRITE */" >>$@
	echo "" >>$@
	echo "/* debugging: show xml data. */" >>$@
	echo "/* #undef ULXR_SHOW_XML */" >>$@
	echo "" >>$@
	echo "/* Define to re-use a server socket (saves time while debugging). */" >>$@
	echo "/* #undef ULXR_REUSE_SOCKET */" >>$@
	echo "" >>$@
	echo "/* use WxWindows strings instead of basic_string. */" >>$@
	echo "/* #undef ULXR_USE_WXSTRING */" >>$@
	echo "" >>$@
	echo "/* Version number of package */" >>$@
	echo "#define ULXR_VERSION \"$(ULXMLRPCPP_VERSION)\"" >>$@
	echo "" >>$@
	echo "/* Version number of package as long int */" >>$@
	echo "#define ULXR_VERSION_NUMBER \"$(ULXR_VERSION_NUMBER)\"" >>$@
	echo "" >>$@
	echo "/* Name of package */" >>$@
	echo "#define ULXR_PACKAGE \"$(PACKAGE)\"" >>$@
	echo "" >>$@
	echo "/* Define if you have std::sprintf */" >>$@
	echo "#define HAVE_STD_SPRINTF 1" >>$@
	echo "" >>$@
	echo "/* Define if you have std::ctime */" >>$@
	echo "#define HAVE_STD_CTIME 1" >>$@
	echo "" >>$@
	test $@ -ef $(srcdir)/$@ || cp $@ $(srcdir)


ulxr_config-bcb5.h:
	perl  -i.bak -pe "s/(.*ulxmlrpcpp)(_)(\d*)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\4/g"  $(top_srcdir)/bcb5/*.bpr
	perl  -i.bak -pe "s/(.*ulxmlrpcpp)(_)(\d*)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\4/g"  $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(.*ulxmlrpcpp)(_)(\d*)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\4/g"  $(top_srcdir)/bcb5/*.bpg
	perl  -i.bak -pe "s/(.*ulxmlrpcpp_contrib)(_)(\d*)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\4/g"  $(top_srcdir)/bcb5/*.bpr
	perl  -i.bak -pe "s/(.*ulxmlrpcpp_contrib)(_)(\d*)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\4/g"  $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(.*ulxmlrpcpp_contrib)(_)(\d*)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\4/g"  $(top_srcdir)/bcb5/*.bpg
	perl  -i.bak -pe "s/(compile\\\\ulxmlrpcpp)(-)(\d*)\.(\d*)/\1-$(ULXMLRPCPP_MAJOR_VERSION).$(ULXMLRPCPP_MINOR_VERSION)/g" $(top_srcdir)/bcb5/*.bpr
	perl  -i.bak -pe "s/(compile\\\\ulxmlrpcpp)(-)(\d*)\.(\d*)/\1-$(ULXMLRPCPP_MAJOR_VERSION).$(ULXMLRPCPP_MINOR_VERSION)/g" $(top_srcdir)/bcb5/*.bat
	perl  -i.bak -pe "s/(compile\\\\ulxmlrpcpp_contrib)(-)(\d*)\.(\d*)/\1-$(ULXMLRPCPP_MAJOR_VERSION).$(ULXMLRPCPP_MINOR_VERSION)/g" $(top_srcdir)/bcb5/*.bpr
	perl  -i.bak -pe "s/(compile\\\\ulxmlrpcpp_contrib)(-)(\d*)\.(\d*)/\1-$(ULXMLRPCPP_MAJOR_VERSION).$(ULXMLRPCPP_MINOR_VERSION)/g" $(top_srcdir)/bcb5/*.bat
	echo "// #define's for Borland BCB5" >$@
	echo "// derived from automatically generated config.h. See Makefile.am." >>$@
	echo "" >>$@
	echo "#pragma warn -8030 // temporary var" >>$@
	echo "" >>$@
	echo "/* Define to turn debugging globally on. */" >>$@
	echo "#undef DEBUG" >>$@
	echo "" >>$@
	echo "/* Define to use log4j logging host. */" >>$@
	echo "#undef ULXR_LOG4J" >>$@
	echo "" >>$@
	echo "/* Define if you have ftime(). */" >>$@
	echo "#define HAVE_FTIME" >>$@
	echo "" >>$@
	echo "/* Define for unicode environment(). */" >>$@
	echo "#undef ULXR_UNICODE" >>$@
	echo "" >>$@
	echo "/* Define to compile only the unicode helper methods. */" >>$@
	echo "#define ULXR_UNICODE_ONLY_HELPERS" >>$@
	echo "" >>$@
	echo "/* Define if you have a multithreaded environment. */" >>$@
	echo "#define ULXR_MULTITHREADED" >>$@
	echo "" >>$@
	echo "/* Define to compile for openssl support. */" >>$@
	echo "#undef ULXR_INCLUDE_SSL_STUFF" >>$@
	echo "" >>$@
	echo "/* Define if you have the <expat.h> header file. */" >>$@
	echo "#define HAVE_EXPAT_H 1" >>$@
	echo "" >>$@
	echo "/* Define if you have the <iconv.h> header file. */" >>$@
	echo "/* #undef HAVE_ICONV_H 1 */" >>$@
	echo "" >>$@
	echo "/* Define as const if the declaration of iconv() needs const. */" >>$@
	echo "#define ICONV_CONST" >>$@
	echo "" >>$@
	echo "/* Define if you have the <sys/socket.h> header file. */" >>$@
	echo "#define HAVE_SYS_SOCKET_H 1" >>$@
	echo "" >>$@
	echo "/* Define if you have the strcase function. */" >>$@
	echo "#undef HAVE_STRCASECMP" >>$@
	echo "" >>$@
	echo "/* root directory for the built-in http server */" >>$@
	echo "#if !defined(ULXR_DATADIR)" >>$@
	echo "# define ULXR_DATADIR \"c:\\\\ulxmlrpcpp\"" >>$@
	echo "#endif" >>$@
	echo "" >>$@
	echo "/* define if WinSocket already started somewhere else. */" >>$@
	echo "/* #undef ULXR_NO_WSA_STARTUP */" >>$@
	echo "" >>$@
	echo "/* define if you want to omit tcp/ip and http stuff. */" >>$@
	echo "/* #undef ULXR_OMIT_TCP_STUFF */" >>$@
	echo "" >>$@
	echo "/* define if you want to automatically create Value()s from int, bool, .. */" >>$@
	echo "/* #undef ULXR_USE_INTRINSIC_VALUE_TYPES */" >>$@
	echo "" >>$@
	echo "/* define if you want to create a Struct::Member like this: ("name" << value) */" >>$@
	echo "/* #undef ULXR_STRUCT_MEMBER_FROM_NAME_VALUE */" >>$@
	echo "" >>$@
	echo "/* debugging: show call trace. */" >>$@
	echo "/* #undef ULXR_SHOW_TRACE */" >>$@
	echo "" >>$@
	echo "/* debugging: show debug information. */" >>$@
	echo "/* #undef ULXR_DEBUG_OUTPUT */" >>$@
	echo "" >>$@
	echo "/* debugging: show transfered data. */" >>$@
	echo "/* #undef ULXR_SHOW_CONTENT */" >>$@
	echo "" >>$@
	echo "/* debugging: show http header contents. */" >>$@
	echo "/* #undef ULXR_SHOW_HTTP */" >>$@
	echo "" >>$@
	echo "/* debugging: show each read characters. */" >>$@
	echo "/* #undef ULXR_SHOW_READ */" >>$@
	echo "" >>$@
	echo "/* debugging: show each written characters. */" >>$@
	echo "/* #undef ULXR_SHOW_WRITE */" >>$@
	echo "" >>$@
	echo "/* debugging: show xml data. */" >>$@
	echo "/* #undef ULXR_SHOW_XML */" >>$@
	echo "" >>$@
	echo "/* Define to re-use a server socket (saves time while debugging). */" >>$@
	echo "/* #undef ULXR_REUSE_SOCKET */" >>$@
	echo "" >>$@
	echo "/* use WxWindows strings instead of basic_string. */" >>$@
	echo "/* #undef ULXR_USE_WXSTRING */" >>$@
	echo "" >>$@
	echo "/* Version number of package */" >>$@
	echo "#define ULXR_VERSION \"$(ULXMLRPCPP_VERSION)\"" >>$@
	echo "" >>$@
	echo "/* Version number of package as long int */" >>$@
	echo "#define ULXR_VERSION_NUMBER \"$(ULXR_VERSION_NUMBER)\"" >>$@
	echo "" >>$@
	echo "/* Name of package */" >>$@
	echo "#define ULXR_PACKAGE \"$(PACKAGE)\"" >>$@
	echo "" >>$@
	echo "/* Define if you have std::sprintf */" >>$@
	echo "#define HAVE_STD_SPRINTF 1" >>$@
	echo "" >>$@
	echo "/* Define if you have std::ctime */" >>$@
	echo "#define HAVE_STD_CTIME 1" >>$@
	echo "" >>$@
	test $@ -ef $(srcdir)/$@ || cp $@ $(srcdir)


ulxr_config-msvc.h:
	perl  -i.bak -pe "s/(.*ulxmlrpcpp_contrib)_(\d+)([d]?)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\3\4/g"     $(top_srcdir)/msvc8/ulxmlrpcpp/examples/*.vcproj $(top_srcdir)/msvc8/ulxmlrpcpp/examples/tutorial/*.vcproj $(top_srcdir)/msvc8/ulxmlrpcpp/*.vcproj $(top_srcdir)/msvc8/ulxmlrpcpp/tests/*.vcproj
	perl  -i.bak -pe "s/(.*ulxmlrpcpp)_(\d+)([d]?)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\3\4/g"             $(top_srcdir)/msvc8/ulxmlrpcpp/examples/*.vcproj $(top_srcdir)/msvc8/ulxmlrpcpp/examples/tutorial/*.vcproj $(top_srcdir)/msvc8/ulxmlrpcpp/*.vcproj $(top_srcdir)/msvc8/ulxmlrpcpp/tests/*.vcproj
	perl  -i.bak -pe "s/(.*ulxmlrpcpp_contrib)_(\d+)([d]?)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\3\4/g"     $(top_srcdir)/msvc71/ulxmlrpcpp/examples/*.vcproj $(top_srcdir)/msvc71/ulxmlrpcpp/examples/tutorial/*.vcproj $(top_srcdir)/msvc71/ulxmlrpcpp/*.vcproj $(top_srcdir)/msvc71/ulxmlrpcpp/tests/*.vcproj
	perl  -i.bak -pe "s/(.*ulxmlrpcpp)_(\d+)([d]?)(\.lib|\.dll)/\1_$(ULXMLRPCPP_LT_CURRENT)\3\4/g"             $(top_srcdir)/msvc71/ulxmlrpcpp/examples/*.vcproj $(top_srcdir)/msvc71/ulxmlrpcpp/examples/tutorial/*.vcproj $(top_srcdir)/msvc71/ulxmlrpcpp/*.vcproj $(top_srcdir)/msvc71/ulxmlrpcpp/tests/*.vcproj
	echo "// #define's for MS Visual C++" >$@
	echo "// derived from automatically generated config.h. See Makefile.am." >>$@
	echo "" >>$@
	echo "/* Define to turn debugging globally on. */" >>$@
	echo "#undef DEBUG" >>$@
	echo "" >>$@
	echo "/* Define to use log4j logging host. */" >>$@
	echo "#undef ULXR_LOG4J" >>$@
	echo "" >>$@
	echo "/* Define if you have ftime(). */" >>$@
	echo "#define HAVE_FTIME" >>$@
	echo "" >>$@
	echo "/* Define to compile for unicode environment. */" >>$@
	echo "#undef ULXR_UNICODE" >>$@
	echo "" >>$@
	echo "/* Define to compile only the unicode helper methods. */" >>$@
	echo "#undef ULXR_UNICODE_ONLY_HELPERS" >>$@
	echo "" >>$@
	echo "/* Define if you have a multithreaded environment. */" >>$@
	echo "#define ULXR_MULTITHREADED" >>$@
	echo "" >>$@
	echo "/* Define to compile for openssl support. */" >>$@
	echo "#undef ULXR_INCLUDE_SSL_STUFF" >>$@
	echo "" >>$@
	echo "/* Define if you have the <expat.h> header file. */" >>$@
	echo "#define HAVE_EXPAT_H 1" >>$@
	echo "" >>$@
	echo "/* Define if you have the <iconv.h> header file. */" >>$@
	echo "/* #undef HAVE_ICONV_H 1 */" >>$@
	echo "" >>$@
	echo "/* Define as const if the declaration of iconv() needs const. */" >>$@
	echo "#define ICONV_CONST" >>$@
	echo "" >>$@
	echo "/* Define if you have the <sys/socket.h> header file. */" >>$@
	echo "#define HAVE_SYS_SOCKET_H 1" >>$@
	echo "" >>$@
	echo "/* Define if you have the strcasecmp function. */" >>$@
	echo "#undef HAVE_STRCASECMP" >>$@
	echo "" >>$@
	echo "/* Define if you have std::sprintf */" >>$@
	echo "#undef HAVE_STD_SPRINTF" >>$@
	echo "" >>$@
	echo "/* Define if you have std::ctime */" >>$@
	echo "#undef HAVE_STD_CTIME" >>$@
	echo "" >>$@
	echo "/* root directory for the built-in http server */" >>$@
	echo "#if !defined(ULXR_DATADIR)" >>$@
	echo "# define ULXR_DATADIR \"c:\\\\ulxmlrpcpp\"" >>$@
	echo "#endif" >>$@
	echo "" >>$@
	echo "/* define if WinSocket already started somewhere else. */" >>$@
	echo "/* #undef ULXR_NO_WSA_STARTUP */" >>$@
	echo "" >>$@
	echo "/* define if you want to omit tcp/ip and http stuff. */" >>$@
	echo "/* #undef ULXR_OMIT_TCP_STUFF */" >>$@
	echo "" >>$@
	echo "/* define if you want to automatically create Value()s from int, bool, .. */" >>$@
	echo "/* #undef ULXR_USE_INTRINSIC_VALUE_TYPES */" >>$@
	echo "" >>$@
	echo "/* define if you want to create a Struct::Member like this: ("name" << value) */" >>$@
	echo "/* #undef ULXR_STRUCT_MEMBER_FROM_NAME_VALUE */" >>$@
	echo "" >>$@
	echo "/* debugging: show call trace. */" >>$@
	echo "/* #undef ULXR_SHOW_TRACE */" >>$@
	echo "" >>$@
	echo "/* debugging: show debug information. */" >>$@
	echo "/* #undef ULXR_DEBUG_OUTPUT */" >>$@
	echo "" >>$@
	echo "/* debugging: show transfered data. */" >>$@
	echo "/* #undef ULXR_SHOW_CONTENT */" >>$@
	echo "" >>$@
	echo "/* debugging: show http header contents. */" >>$@
	echo "/* #undef ULXR_SHOW_HTTP */" >>$@
	echo "" >>$@
	echo "/* debugging: show each read characters. */" >>$@
	echo "/* #undef ULXR_SHOW_READ */" >>$@
	echo "" >>$@
	echo "/* debugging: show each written characters. */" >>$@
	echo "/* #undef ULXR_SHOW_WRITE */" >>$@
	echo "" >>$@
	echo "/* debugging: show xml data. */" >>$@
	echo "/* #undef ULXR_SHOW_XML */" >>$@
	echo "" >>$@
	echo "/* Define to re-use a server socket (saves time while debugging). */" >>$@
	echo "/* #undef ULXR_REUSE_SOCKET */" >>$@
	echo "" >>$@
	echo "/* use WxWindows strings instead of basic_string. */" >>$@
	echo "/* #undef ULXR_USE_WXSTRING */" >>$@
	echo "" >>$@
	echo "/* Version number of package */" >>$@
	echo "#define ULXR_VERSION \"$(ULXMLRPCPP_VERSION)\"" >>$@
	echo "" >>$@
	echo "/* Version number of package as long int */" >>$@
	echo "#define ULXR_VERSION_NUMBER \"$(ULXR_VERSION_NUMBER)\"" >>$@
	echo "" >>$@
	echo "/* Name of package */" >>$@
	echo "#define ULXR_PACKAGE \"$(PACKAGE)\"" >>$@
	echo "" >>$@
	test $@ -ef $(srcdir)/$@ || cp $@ $(srcdir)


i18n_files = $(libulxmlrpcpp_la_SOURCES)

messages:
	xgettext `ls $(i18n_files)` -C -kULXR_I18N_NOOP -kulxr_i18n -kULXR_PCHAR -o po/$(PACKAGE).pot

