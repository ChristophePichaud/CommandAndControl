SUBDIRS       = . contrib

INCLUDES      = -I.  -I$(top_srcdir) $(all_includes) $(EA_EXTRA_INC)

CLEANFILES    = *.~* *.~~* *~

AM_CXXFLAGS   = -DULXR_HAVE_CONFIG_H

EXTRA_DIST    = val1_suite crypt.cpp foo-cert.pem.in readme gen_allhex.c \
                stressing-client.sh.in stressing-server.sh.in util.c

LDADD         = ../libulxmlrpcpp.la
AM_LDFLAGS    = $(all_libraries) $(LIBPTHREAD) $(LIBEXPAT) $(LIBICONV) $(LIBSOCKET)  $(EA_EXTRA_LIB)  $(PTHREAD_CFLAGS)

.PHONY: valgrind-check

###################################################

#                 sym_client_main sym_server_main 
check_PROGRAMS = xmlbench xmlfunc wbxmlfunc\
                 val1_client val1_server \
                 http_client http_server \
                 https_client https_server \
                 rpc_client rpc_server \
                 mt_client mt_server \
                 secure_client secure_server \
                 httpfunc \
                 base64 resource

SINGLE_TESTS = xmlfunc wbxmlfunc httpfunc base64 xmlbench resource
TESTS        = $(SINGLE_TESTS)

xmlbench_SOURCES             = xmlbench.cpp
wbxmlfunc_SOURCES            = wbxmlfunc.cpp
xmlfunc_SOURCES              = xmlfunc.cpp
http_client_SOURCES          = http_client.cpp
http_server_SOURCES          = http_server.cpp
https_client_SOURCES         = https_client.cpp
https_server_SOURCES         = https_server.cpp
rpc_client_SOURCES           = rpc_client.cpp
rpc_server_SOURCES           = rpc_server.cpp
mt_client_SOURCES            = mt_client.cpp
mt_server_SOURCES            = mt_server.cpp
#sym_client_main_SOURCES      = sym_client_main.cpp
#sym_server_main_SOURCES      = sym_server_main.cpp

# crypt.cpp
secure_client_SOURCES   = secure_client.cpp

# crypt.cpp
secure_server_SOURCES   = secure_server.cpp

val1_client_SOURCES     = val1_client.cpp
val1_server_SOURCES     = val1_server.cpp
httpfunc_SOURCES        = httpfunc.cpp
base64_SOURCES          = base64.cpp
resource_SOURCES        = resource.cpp


VALGRIND_OPTS=--show-reachable=yes --leak-check=yes -v
MPATROL_OPTS=-d --leak-table
VG_SUPPRESS=vg-suppress-`hostname`.txt

base64_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
httpfunc_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
wbxmlfunc_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
xmlfunc_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
xmlbench_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
resource_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++

secure_server_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
secure_client_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++

mt_server_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
mt_client_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++

rpc_server_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
rpc_client_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++

https_server_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
https_client_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++

http_server_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
http_client_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++

val1_server_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++
val1_client_LDADD = $(top_builddir)/ulxmlrpcpp/libulxmlrpcpp.la -lstdc++

