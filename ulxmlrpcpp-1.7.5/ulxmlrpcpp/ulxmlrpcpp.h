/***************************************************************************
              ulxmlrpcpp.h  -  common stuff for xml-rpc project
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxmlrpcpp.h 1057 2007-08-10 20:30:19Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

/**
  * @libdoc ultra lightweight XML RPC library for C++
  *
  * @p ulxmlrpcpp is a lightweight library to perform calls on remote systems
  * using the xml protocol.
  */


#include <ulxmlrpcpp/ulxr_log4j.h>

#ifndef ULXR_H
#define ULXR_H


////////////////////////////////////////////////////////////
// Only here for quick-n-dirty testing purposes.
// Use the various config-* files for production code.

// #define ULXR_LOG4J

// #define ULXR_ENFORCE_NON_PERSISTENT
// #define ULXR_STRUCT_MEMBER_FROM_NAME_VALUE
// #define ULXR_USE_INTRINSIC_VALUE_TYPES
// #define ULXR_ENABLE_GET_PEERNAME
// #define ULXR_USE_STD_EXCEPTION

// #define DEBUG
// #define ULXR_DEBUG_OUTPUT

// #define ULXR_SHOW_TRACE
// #define ULXR_SHOW_XML
// #define ULXR_SHOW_HTTP
// #define ULXR_SHOW_WRITE
// #define ULXR_SHOW_CONTENT
// #define ULXR_SHOW_READ

//
////////////////////////////////////////////////////////////


#ifdef _MSC_VER
# define _CRT_SECURE_NO_DEPRECATE
# include <ulxmlrpcpp/ulxr_config-msvc.h>
#elif defined(__BORLANDC__)
# include <ulxmlrpcpp/ulxr_config-bcb5.h>
#elif defined(__CYGWIN__)
# include <ulxmlrpcpp/ulxr_config-cygwin.h>
#else
# ifdef ULXR_HAVE_CONFIG_H
#  include <config.h>     // while generating lib
# else
#  include <ulxmlrpcpp/ulxr_config.h>  // while generating an app using ulxr
# endif
#endif


# ifdef _MSC_VER
// Compilation Warning remover, used to skip some STL compilation warnings
#  pragma warning (disable:4786)  // this warning occurs when VC6 truncate a debug symbol which is longer than 255 chars.
#  pragma warning (disable:4251)  // this warning occurs when a class member doesn't have a dll-interface.
#  pragma warning (disable:4180)
#  pragma warning (disable:4355)
# endif // _MSC_VER


#if defined(HAVE_ICONV_H) || defined(HAVE_ICONV)
# include <iconv.h>
#endif

//////////////////////////////////////////////////
// Catch Macros from MACOS

#ifdef verify
#error "<verify> must not be #defined within mockpp sources"
#endif

#ifdef check
#error "<check> must not be #defined within mockpp sources"
#endif

#ifdef require
#error "<require> must not be #defined within mockpp sources"
#endif

//! ulxr::Exception is derived from std::exception
#define ULXR_USE_STD_EXCEPTION

#ifdef ULXR_USE_WXSTRING

# ifdef _MSC_VER
#  pragma message("Using wxString")
# endif

# define __WXMSW__
# define __WXDEBUG__
# define WXUSINGDLL

# include <wx/string.h>

/** @namespace ulxr
  * @brief Namespace for project  "Ultra Lightwight Xml Rpc Library".
  */

namespace ulxr
{

  typedef wxwindows::string   Cpp8BitString;
#if defined (ULXR_UNICODE) || defined(ULXR_UNICODE_ONLY_HELPERS)
  typedef wxwindows::wstring  Cpp16BitString;
#endif

} // namespace ulxr

# ifdef _MSC_VER
#  pragma comment( lib, "wx23_1d" ) // auto include the wxWindows 2.3.1
# endif

#else // ULXR_USE_WXSTRING

# ifdef _MSC_VER
#  pragma message("Using std::string")
# endif

# if defined(__BORLANDC__) || defined (_MSC_VER)
#  define HAVE_WSTRING
# endif

# include <string>

#ifdef __BORLANDC__
# define ULXR_INT64 INT64
#elif defined(_MSC_VER)
# define ULXR_INT64 __int64
#else
# define ULXR_INT64 __int64_t
#endif

namespace ulxr
{
#if defined (ULXR_UNICODE) || defined(ULXR_UNICODE_ONLY_HELPERS)

# if !defined(HAVE_WSTRING)
  typedef std::basic_string<wchar_t> Cpp16BitString;
# else
  typedef std::wstring               Cpp16BitString;
# endif

#endif

  typedef std::string                Cpp8BitString;
} // namespace ulxr

#endif // ULXR_USE_WXSTRING


#ifdef __sun__
#ifndef __SUN__
#define __SUN__
#endif
#endif

#if defined(__BORLANDC__) || defined (_MSC_VER)

# ifndef __WIN32__
#  define __WIN32__
# endif //__WIN32__

# ifdef __BORLANDC__
#  include <io.h>

# ifndef __MT__
# error MultiThreaded compilation is HIGHLY recommended, change at own risk
# endif

# ifndef _RTLDLL
# error Linking with dynamic RTL is HIGHLY recommended, change at own risk
# endif

# endif

# if defined (_MSC_VER)

// STATIC
#  if defined(ULXR_STATIC_LIB)
#   define ULXR_API_DECL(type) type
#   define ULXR_API_DECL0
#   define ULXR_API_IMPL(type) type
#   define ULXR_API_IMPL0
#   pragma message("*** ULXMLRPCPP: Static ***")
#  else //ULXR_STATIC_LIB

#  if defined(ULXR_NEED_EXPORTS)
#   define ULXR_API_DECL(type)  __declspec(dllexport)  type
#   define ULXR_API_DECL0       __declspec(dllexport)
#   define ULXR_API_IMPL(type)  __declspec(dllexport)  type
#   define ULXR_API_IMPL0       __declspec(dllexport)
#   pragma message("*** ULXMLRPCPP: DllExport ***")

#  else // ULXR_NEED_EXPORTS

#   define ULXR_API_DECL(type)  __declspec(dllimport) type
#   define ULXR_API_DECL0       __declspec(dllimport)
#   define ULXR_API_IMPL(type)  __declspec(dllimport) type
#   define ULXR_API_IMPL0       __declspec(dllimport)
#   pragma message("*** ULXMLRPCPP: DllImport ***")
#  endif // ULXR_NEED_EXPORTS

#  endif //ULXR_STATIC_LIB

# else // __BORLANDC__

# if defined(ULXR_NEED_EXPORTS)
#  define ULXR_API_DECL0      __declspec(dllexport)
#  define ULXR_API_IMPL0      __declspec(dllexport)
#  define ULXR_API_DECL(type) __declspec(dllexport) type
#  define ULXR_API_IMPL(type) __declspec(dllexport) type
# else
#  define ULXR_API_DECL0      __declspec(dllimport)
#  define ULXR_API_IMPL0
#  define ULXR_API_DECL(type) __declspec(dllimport) type
#  define ULXR_API_IMPL(type)  type
# endif

# endif

#else // __BORLANDC__ || _MSC_VER

# include <unistd.h>
# define ULXR_API_DECL0
# define ULXR_API_IMPL0
# define ULXR_API_DECL(type) type
# define ULXR_API_IMPL(type) type

#endif


#if defined(ULXR_DEBUG_OUTPUT)

#include <iostream>
#include <sstream>

# if defined (_MSC_VER)

#  define ULXR_DTRACE(x)    { ULXR_CERR << ULXR_GET_STRING(__FILE__) << "," << __LINE__ << ":" << x << ULXR_PCHAR("\n"); }
#  define ULXR_DOUT(x)      { ULXR_CERR << x  << ULXR_PCHAR("\n"); }
#  define ULXR_DWRITE(b, l) { ULXR_CERR << "ULXR_DWRITE not yet implemented" <<  ULXR_PCHAR("\n"); }
                           //{ ::write(2, b, l); std::cerr << "\n";}
# else //_MSC_VER

#ifdef ULXR_LOG4J

#  define ULXR_DTRACE(x) \
   { if (ulxr::getLogger4J() != 0) \
     {  \
       ulxr::CppOStringStream os; \
       os << x; \
       ulxr::getLogger4J()->send(ULXR_GET_STRING(ULXR_PCHAR("TRACE")), os.str(), ULXR_GET_STRING(__FILE__), __LINE__ ); \
      } \
   }

#  define ULXR_DOUT(x) \
   {  if (ulxr::getLogger4J() != 0) \
      {  \
        ulxr::CppOStringStream os; \
        os << x; \
        ulxr::getLogger4J()->send(ULXR_GET_STRING(ULXR_PCHAR("DEBUG")), os.str() ); \
      } \
   }

#  define ULXR_DWRITE(b, l) \
   { if (ulxr::getLogger4J() != 0) \
      {  \
        std::string s0(b, l); \
        ulxr::getLogger4J()->send(ULXR_GET_STRING(ULXR_PCHAR("DEBUG")), ULXR_GET_STRING(s0)); \
      } \
   }

#else

#  define ULXR_DTRACE(x)     { ULXR_CERR << ULXR_GET_STRING(__FILE__) << "," << __LINE__ << ": " <<  x << ULXR_PCHAR("\n"); }
#  define ULXR_DOUT(x)      { ULXR_CERR << x  << ULXR_PCHAR("\n"); }
#  define ULXR_DWRITE(b, l) { ::write(2, b, l); ULXR_CERR << ULXR_PCHAR("\n");}

#endif

# endif //_MSC_VER

#else // DEBUG

# define ULXR_DWRITE(b, l)
# define ULXR_DTRACE(x)
# define ULXR_DOUT(x)

#endif // DEBUG

/////////////////////////////////////////////////////////////
// quickly enable debug output without source changes

// trace output
#ifdef  ULXR_SHOW_TRACE
#define ULXR_TRACE(x)        ULXR_DTRACE(x)
#else
#define ULXR_TRACE(x)
#endif

// xml body
#ifdef  ULXR_SHOW_XML
#define ULXR_DOUT_XML(x)        ULXR_DOUT(ULXR_PCHAR("xml:\n") << x)
#else
#define ULXR_DOUT_XML(x)
#endif

// http header
#ifdef  ULXR_SHOW_HTTP
#define ULXR_DOUT_HTTP(x)       ULXR_DOUT(x)
#else
#define ULXR_DOUT_HTTP(x)
#endif

// http content
#ifdef  ULXR_SHOW_CONTENT
#define ULXR_DOUT_REQ(x)      ULXR_DOUT(x)
#define ULXR_DOUT_RESP(x)     ULXR_DOUT(x)
#else
#define ULXR_DOUT_REQ(x)
#define ULXR_DOUT_RESP(x)
#endif

// conn:write
#ifdef  ULXR_SHOW_WRITE
#define ULXR_DOUT_WRITE(x)      ULXR_DOUT(x)
#define ULXR_DWRITE_WRITE(p,l)  ULXR_DWRITE(p,l)
#else
#define ULXR_DOUT_WRITE(x)
#define ULXR_DWRITE_WRITE(p,l)
#endif

// conn:read
#ifdef  ULXR_SHOW_READ
#define ULXR_DOUT_READ(x)       ULXR_DOUT(x)
#define ULXR_DWRITE_READ(p,l)   ULXR_DWRITE(p,l)
#else
#define ULXR_DOUT_READ(x)
#define ULXR_DWRITE_READ(p,l)

#endif


/////////////////////////////////////////////////////////////
//

#define ULXR_RECV_BUFFER_SIZE  50    // keep rather small, otherwise two messages
                                     // might be read as a single block
#define ULXR_SEND_BUFFER_SIZE  2000

namespace ulxr {

  class Log4JSender;

// officially reserved -32768 .. -32000

// parse errors
const int NotWellformedError           = -32700;
const int UnsupportedEncodingError     = -32701;
const int InvalidCharacterError        = -32702;

// server errors
const int NotConformingError           = -32600;
const int MethodNotFoundError          = -32601;
const int InvalidMethodParameterError  = -32602;
const int InternalXmlRpcError          = -32603;

// misc errors
const int ApplicationError             = -32500;
const int SystemError                  = -32400;
const int TransportError               = -32300;

#ifdef ULXR_UNICODE

  typedef std::wostringstream  CppOStringStream;
  typedef Cpp16BitString       CppString;
  typedef wchar_t              Char;

//! make the charactor constant of type wchar_t
# define ULXR_CHAR(x)  L##x

//! make the string literal a wchar_t literal
# define ULXR_PCHAR(x) L##x

# define ULXR_COUT   std::wcout
# define ULXR_CERR   std::wcerr

#else

  typedef std::ostringstream  CppOStringStream;
  typedef std::string         CppString;
  typedef char                Char;
//! make the charactor constant of type char
# define ULXR_CHAR(x)   x

//! make the string literal a char literal
# define ULXR_PCHAR(x)  x

# define ULXR_COUT   std::cout
# define ULXR_CERR   std::cerr

#endif


/** Gets the various parts of the version number.
  * @param  major  major part
  * @param  minor  minor part
  * @param  patch  patch counter
  * @param  debug  true: compiled with DEBUG
  * @param  info   some human readable information
  */
  ULXR_API_DECL(void) getVersion (int &major, int &minor, int &patch, bool &debug, CppString &info);

/** Removes trailing and leading white space.
  * @param  s  input string
  * @return  output string
  */
  ULXR_API_DECL(CppString) stripWS(const CppString &s);

/** Converts binary data into base64 encoding.
  * @param  normstr   the raw data
  * @param  add_crlf  add cr/lf after each 78 characters to limit line length
  * @return  encoded string
  */
  ULXR_API_DECL(CppString) encodeBase64(const CppString &normstr, bool add_crlf = true);

/** Converts a base64 encoded string into raw data.
  * @param  b64str    encoded string
  * @param  errcheck  true: throw exception in cas of error
  * @return the raw data
  */
  ULXR_API_DECL(CppString) decodeBase64(const CppString &b64str, bool errcheck = true);

 /** Replaces dangerous sequences in xml strings.
   * Several characters have a special meaning in xml and must be
   * converted to not disturb parsing.
   * @param  str   the string to escape
   * @param  supp  supress non-unicode characterss like #0 or 0x0c
   * @return the converted string
   */
   ULXR_API_DECL(CppString) xmlEscape(const CppString &str, bool supp = false);

 /** Re-converts strings after being parsed.
   * @param  str   the string to escape
   * @return the converted string
   */
   ULXR_API_DECL(CppString) xmlUnEscape(const CppString &str);

 /** Converts string into lowercase.
   * Since the C-library is used this is only valid for ascii characters
   * @param  str   pointer to the string to convert
   */
   ULXR_API_DECL(void) makeLower(CppString &str);

 /** Converts string into uppercase.
   * Since the C-library is used this is only valid for ascii characters
   * @param  str   pointer to the string to convert
   */
   ULXR_API_DECL(void) makeUpper(CppString &str);

#if defined (ULXR_UNICODE) || defined(ULXR_UNICODE_ONLY_HELPERS)

/** Returns a string in utf8 as 16bit Unicode.
   * @param  val string in utf8
   * @return current value
   * @ingroup grp_ulxr_unicode_cvt
   */
  ULXR_API_DECL(Cpp16BitString) utf8ToUnicode(const Cpp8BitString &val);

 /** Returns a string in 16bit Unicode as utf8.
   * @param  val wstring in unicode
   * @return utf8 string
   * @ingroup grp_ulxr_unicode_cvt
   */
   ULXR_API_DECL(Cpp8BitString) unicodeToUtf8(const Cpp16BitString &val);

#endif

 /** Returns a string in 8bit extended ascii as utf8.
   * This function transforms every single 8bit wide character into its
   * utf8 equivalent. Needed because uxmlrpcpp internally works either in unicode or utf8
   * depending on \c ULXR_UNICODE.
   * @attention Meaningful only when reverted with \c utf8ToAscii().
   * @param  val string in 8bit encoding
   * @return utf8 string
   * @ingroup grp_ulxr_unicode_cvt
   */
   ULXR_API_DECL(Cpp8BitString) asciiToUtf8(const Cpp8BitString &val);

 /** Returns a string in utf8 as 8bit ascii.
   * This function transforms an utf8 string into its extended 8bit wide string
   * equivalent. Needed because uxmlrpcpp internally works either in unicode or utf8
   * depending on \c ULXR_UNICODE.
   * @attention Meaningful only when previously converted with \c asciiToUtf8().
   * @param  val string in utf8
   * @return current value
   * @ingroup grp_ulxr_unicode_cvt
   */
  ULXR_API_DECL(Cpp8BitString) utf8ToAscii(const Cpp8BitString &val);

 /** Returns a character in 16bit Unicode as utf8.
   * @param  c   unicode character
   * @return utf8 string
   * @ingroup grp_ulxr_unicode_cvt
   */
   ULXR_API_DECL(Cpp8BitString) unicodeToUtf8(const unsigned c);

#if defined(HAVE_ICONV_H) || defined(HAVE_ICONV)

 /** Returns a string in a user encoding as utf8.
   * Works only good with an 8bit encoding as "string" is used for both input
   * and output.
   * @param  val string in user encoding
   * @param  encoding  name of user encoding according to "man 3 iconv_open"
   * @return utf8 string
   * @ingroup grp_ulxr_unicode_cvt
   */
   ULXR_API_DECL(Cpp8BitString) encodingToUtf8(const Cpp8BitString &val, const char *encoding);

 /** Returns a string in utf8 as user encoding.
   * Works only good with an 8bit encoding as "string" is used for both input
   * and output.
   * @param  val string in utf8
   * @param  encoding  name of user encoding according to "man 3 iconv_open"
   * @return string in user encoding
   * @ingroup grp_ulxr_unicode_cvt
   */
   ULXR_API_DECL(Cpp8BitString) utf8ToEncoding(const Cpp8BitString &val, const char *encoding);

 /** Returns a string converted from and to a user encoding.
   * Useful when you have to convert a lot of strings as you have to generate
   * the conversion descriptor only once. See "man 3 iconv_open()".
   * Works only good with two 8bit encodings as "string" is used for both input
   * and output.
   * @param  val input string in first user encoding
   * @param  con conversion descriptor
   * @return string in second user encoding
   * @ingroup grp_ulxr_unicode_cvt
   */
   ULXR_API_DECL(Cpp8BitString) convertEncoding(const Cpp8BitString &val, iconv_t con);

#endif


#if defined (ULXR_UNICODE) || defined(ULXR_UNICODE_ONLY_HELPERS)

/** Converts a string from latin1 to unicode.
  * Since latin1 is the first unicode page the charcters are just
  * "expanded" from "char" to "wchar_t".
  * @param  latin1  string in ISO8859-1 encoding
  * @return the string in unicode
  * @ingroup grp_ulxr_unicode_cvt
  */
  ULXR_API_DECL(Cpp16BitString) getUnicode(const std::string &latin1);


/** Converts a string from unicode to unicode.
  * Dummy function to avoid error messages.
  * @param  uni string in unicode
  * @return the same string
  * @ingroup grp_ulxr_unicode_cvt
  */
  inline ULXR_API_DECL(Cpp16BitString) getUnicode(const Cpp16BitString &uni)
  {
    return uni;
  }

/** Converts a string from unicode to latin1.
  * Since latin1 is the first unicode page the charcters are just
  * "reduced" from "wchar_t" to "char". Characters beyond this range are undefined.
  * @param  uni string in unicode
  * @return the string in IOS8859-1 encoding
  * @ingroup grp_ulxr_unicode_cvt
  */
  ULXR_API_DECL(std::string) getLatin1(const Cpp16BitString &uni);

#endif

/** Converts a string from latin1 to latin1.
  * Dummy function to avoid error messages.
  * @param  latin1  string in ISO8859-1 encoding
  * @return the same string
  * @ingroup grp_ulxr_unicode_cvt
  */
  inline ULXR_API_DECL(std::string) getLatin1(const std::string &latin1)
  {
    return latin1;
  }

/** Converts a string in a wbxml inline string sequence.
  * @param  inp  inline string
  * @return wbxml sequence
  */
  ULXR_API_DECL(std::string) getWbXmlString(const CppString &inp);

/** Converts a wbxml inline string sequence to a CppString.
  * The start of an inline string sequence is expected in the input. This
  * sequence is transformed and removed from the input.
  * @param  inp  reference to inline string
  * @return wbxml sequence
  */
  ULXR_API_DECL(CppString) wbXmlToString(std::string &inp);

/** Converts an integer in a wbxml multibyte uint32 sequence.
  * @param  i   value to convert
  * @return wbxml sequence
  */
  ULXR_API_DECL(std::string) getWbXmlExtInt(long int i);

/** Converts a wbxml multibyte sequence to an integer.
  * The start of an inline multibyte sequence is expected in the input. This
  * sequence is transformed and removed from the input.
  * @param  inp  reference to multibyte sequnce
  * @return wbxml sequence
  */
  ULXR_API_DECL(long int) wbXmlToInt(std::string &inp);

/** Converts a binary string into visible output in hex
  * @param  s  input data in binary
  * @return hex output
  */
  ULXR_API_DECL(CppString) binaryDebugOutput (const std::string &s);

/** Returns a readable string about the cause of the last system error.
  * Simply calls nonreentrant \c std::strerror() in a thread safe mode.
  * @param  errornum   error number taken from \c errno
  * @return error message
  */
  ULXR_API_DECL(CppString) getLastErrorString(unsigned errornum);

/** Initializes the global log4j object
  * @param  appname  application name. May directly come from argv[0] with full path.
  * @param  loghost  Name of log host. Port is always 4448.
  */
  ULXR_API_DECL(void) intializeLog4J(const std::string &appname,
                                     const CppString &loghost = ULXR_PCHAR("localhost"));

/** Gets a pointer to the global log4j object
  * @return 0 if not avaliable
  * @return != 0 pointer to object
  */
  ULXR_API_DECL(Log4JSender *) getLogger4J();

/** Returns a random number.
  * Simply calls nonreentrant \c std::rand() in a thread safe mode.
  * @return random number
  */
  ULXR_API_DECL(int) getRand();

/** Enables pretty-print of xml repsponses for better readability
  * @param  pretty   true: add spaces and linefeeds for better readability
  */
  ULXR_API_DECL(void) enableXmlPrettyPrint(bool pretty);

/** Gets a string with the line feed for an xml response
  * @return empty if pretty-print disabled
  * @return string for a regular line feed otherwise
  */
  ULXR_API_DECL(CppString) getXmlLinefeed();

/** Gets a string with spaces for an indentation in an xml response
  * @param  indent   number of spaces to indent
  * @return empty if pretty-print disabled
  * @return string with \c indent spaces
  */
  ULXR_API_DECL(CppString) getXmlIndent(unsigned indent);

  class Mutex;

#ifndef ULXR_OMIT_REENTRANT_PROTECTOR
  // protect non-reentrant system library calls
  extern ULXR_API_DECL(Mutex) strerrorMutex;         //!< help to protect non-reentrant std::strerror()
  extern ULXR_API_DECL(Mutex) ctimeMutex;            //!< help to protect non-reentrant std::ctime()
  extern ULXR_API_DECL(Mutex) localtimeMutex;        //!< help to protect non-reentrant std::localtime()
  extern ULXR_API_DECL(Mutex) gethostbynameMutex;    //!< help to protect non-reentrant std::gethostbyname()
  extern ULXR_API_DECL(Mutex) gethostbyaddrMutex;    //!< help to protect non-reentrant std::gethostbyaddr()
  extern ULXR_API_DECL(Mutex) randMutex;             //!< help to protect non-reentrant std::rand()
#endif

}  // namespace ulxr


#ifdef ULXR_DOXYGEN_SHOULD_INCLUDE_THIS

/** Define this macro to omit mutex code to protect non-reentrant system library
  * calls in a threaded environment within ulxmlrpcpp code.
  */
#define ULXR_OMIT_REENTRANT_PROTECTOR

#endif


//! transform a string literal into a ulxr::CppString
#define ULXR_STRING(x) ulxr::CppString(x)

#ifndef ULXR_I18N_NOOP
//! Dummy, tags a string for translation without doing anything
#define ULXR_I18N_NOOP(x) x
#endif

#ifndef ulxr_i18n
//! Dummy, prepare automatic translation within the code
#define ulxr_i18n(x) x
#endif


#if defined (ULXR_UNICODE)
//! transform the string into unicode if needed
# define ULXR_GET_STRING(x) ulxr::getUnicode(x)
#else
//! transform the string into ascii if needed
# define ULXR_GET_STRING(x) ulxr::getLatin1(x)
#endif


/** @def ULXR_NEED_EXPORTS
  * Enable the creation of module exports. Otherwise imports are created.
  */

/** @def ULXR_API_DECL0
  * Returns a platform dependent symbol to export names to the linker.
  */

/** @def ULXR_API_DECL
  * Returns a platform dependent symbol to export names to the linker.
  */

/** @def ULXR_API_IMPL
  * Returns a platform dependent symbol to import names to the linker.
  */

/** @def ULXR_API_IMPL0
  * Returns a platform dependent symbol to import names to the linker.
  */

#ifdef __unix__
#define ULXR_DIRSEP ULXR_PCHAR("/")
#else
#define ULXR_DIRSEP  ULXR_PCHAR("\\")
#endif

#if defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ < 3))
# define ulxr_FILE       FILE
# define ulxr_isspace    isspace
# define ulxr_tolower    tolower
# define ulxr_isdigit    isdigit
# define ulxr_toupper    toupper
# define ulxr_fopen      fopen
# define ulxr_abs        abs
# define ulxr_fabs       fabs
# define ulxr_atoi       atoi
# define ulxr_eof        eof
# define ulxr_feof       feof
# define ulxr_ferror     ferror
# define ulxr_fputc      fputc
# define ulxr_fread      fread
# define ulxr_fwrite     fwrite

#if defined(_MSC_VER)
# define ulxr_access     _access
#else
# define ulxr_access     access
#endif

# define ulxr_remove     remove
# define ulxr_fclose     fclose
# define ulxr_sprintf    sprintf

#if defined(_MSC_VER)
# define ulxr_snprintf   _snprintf
#else
# define ulxr_snprintf   snprintf
#endif

# define ulxr_swprintf   swprintf
# define ulxr_time_t     time_t
# define ulxr_tm         tm
# define ulxr_ctime      ctime
# define ulxr_localtime  localtime
# define ulxr_time       time
# define ulxr_stricmp    stricmp

#else

# define ulxr_FILE       std::FILE
# define ulxr_toupper    std::toupper
# define ulxr_isspace    std::isspace
# define ulxr_tolower    std::tolower
# define ulxr_isdigit    std::isdigit
# define ulxr_fopen      std::fopen
# define ulxr_eof        std::eof
# define ulxr_abs        std::abs
# define ulxr_fabs       std::fabs
# define ulxr_atoi       std::atoi
# define ulxr_feof       std::feof
# define ulxr_ferror     std::ferror
# define ulxr_fputc      std::fputc
# define ulxr_fread      fread
# define ulxr_fwrite     fwrite
# define ulxr_access     access
# define ulxr_remove     std::remove
# define ulxr_fclose     std::fclose
# define ulxr_sprintf    std::sprintf

#if defined(__CYGWIN__)
# define ulxr_snprintf   snprintf
#else
# define ulxr_snprintf   std::snprintf
#endif

# define ulxr_swprintf   std::swprintf
# define ulxr_time_t     std::time_t
# define ulxr_ctime      std::ctime
# define ulxr_localtime  std::localtime
# define ulxr_time       std::time
# define ulxr_tm         std::tm
# define ulxr_stricmp    std::stricmp
#endif


#if defined (_MSC_VER)

#include <iosfwd>

inline std::ostream& operator<< (std::ostream& strm, const ulxr::Cpp8BitString &s)
{
  char* p=(char*)s.c_str();
  return (strm << p);
}

#if defined (ULXR_UNICODE) || defined(ULXR_UNICODE_ONLY_HELPERS)

inline std::wostream& operator<< (std::wostream& strm, const ulxr::Cpp16BitString &s)
{
  wchar_t* p=(wchar_t*)s.c_str();
  return (strm << p);
}

#endif

#endif

#endif // ULXR_H
