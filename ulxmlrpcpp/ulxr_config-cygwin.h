// #define's for Cygwin
// derived from automatically generated config.h. See Makefile.am.

/* Define to turn debugging globally on. */
#undef DEBUG

/* Define to use log4j logging host. */
#undef ULXR_LOG4J

/* Define to compile for unicode environment. */
#undef ULXR_UNICODE

/* Define to compile only the unicode helper methods. */
#undef ULXR_UNICODE_ONLY_HELPERS

/* Define to compile for openssl support. */
#undef ULXR_INCLUDE_SSL_STUFF

/* Define if you have a multithreaded environment. */
#define ULXR_MULTITHREADED

/* Define if you have the <expat.h> header file. */
#define HAVE_EXPAT_H 1

/* Define if you have the <iconv.h> header file. */
/* #undef HAVE_ICONV_H 1 */

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST

/* Define if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the strcase function. */
#undef HAVE_STRCASECMP

/* root directory for the built-in http server */
#if !defined(ULXR_DATADIR)
# define ULXR_DATADIR "c:\\ulxmlrpcpp"
#endif

/* define if WinSocket already started somewhere else. */
/* #undef ULXR_NO_WSA_STARTUP */

/* define if you want to omit tcp/ip and http stuff. */
/* #undef ULXR_OMIT_TCP_STUFF */

/* define if you want to automatically create Value()s from int, bool, .. */
/* #undef ULXR_USE_INTRINSIC_VALUE_TYPES */

/* define if you want to create a Struct::Member like this: (name << value) */
/* #undef ULXR_STRUCT_MEMBER_FROM_NAME_VALUE */

/* debugging: show call trace. */
/* #undef ULXR_SHOW_TRACE */

/* debugging: show debug information. */
/* #undef ULXR_DEBUG_OUTPUT */

/* debugging: show transfered data. */
/* #undef ULXR_SHOW_CONTENT */

/* debugging: show http header contents. */
/* #undef ULXR_SHOW_HTTP */

/* debugging: show each read characters. */
/* #undef ULXR_SHOW_READ */

/* debugging: show each written characters. */
/* #undef ULXR_SHOW_WRITE */

/* debugging: show xml data. */
/* #undef ULXR_SHOW_XML */

/* Define to re-use a server socket (saves time while debugging). */
/* #undef ULXR_REUSE_SOCKET */

/* use WxWindows strings instead of basic_string. */
/* #undef ULXR_USE_WXSTRING */

/* Version number of package */
#define ULXR_VERSION "1.7.5"

/* Version number of package as long int */
#define ULXR_VERSION_NUMBER "0x00010705"

/* Name of package */
#define ULXR_PACKAGE "ulxmlrpcpp"

/* Define if you have std::sprintf */
#define HAVE_STD_SPRINTF 1

/* Define if you have std::ctime */
#define HAVE_STD_CTIME 1

