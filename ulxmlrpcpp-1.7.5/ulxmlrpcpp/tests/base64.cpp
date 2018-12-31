/***************************************************************************
                 base64.cpp  --  test file for string checks

    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: base64.cpp 990 2007-07-14 15:00:39Z ewald-arnold $

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

#include <ulxmlrpcpp/ulxmlrpcpp.h>

#include <cstdio>
#include <ctime>
#include <iostream>
#include <cstring>

#include <ulxmlrpcpp/ulxr_value.h>
#include <ulxmlrpcpp/ulxr_except.h>


int dtable[256];	      /* Encode / decode table */
bool success = true;

void check_malformed(const ulxr::CppString &s)
{
     try
     {
       ulxr::CppString s0 = ulxr::xmlUnEscape(s); // throw
       ULXR_COUT << ULXR_PCHAR("incorrectly converted malformed xml string: ") << s << std::endl;
       success = false;
     }
     catch(...)
     {
        ULXR_COUT << ULXR_PCHAR("correctly caught exception due to malformed xml string: ") << s << std::endl;
     }
}

int main(int argc, char * argv [])
{
  try
  {
     ulxr::intializeLog4J(argv[0]);

     ULXR_COUT << ULXR_PCHAR("sizeof(<string>): " << sizeof(ulxr::Char) << std::endl);

     ulxr_time_t tim = ulxr_time(0);
     ulxr::DateTime dt(tim);
     ULXR_COUT << ULXR_PCHAR("Current time as DateTime: ") << dt.getDateTime() << std::endl;

     ulxr::CppString s0;
     for (unsigned ii = 0; ii < 256; ++ii)
       s0 += ii;
//     ULXR_COUT << s0 << std::endl;

     ulxr::CppString regression =
        ULXR_PCHAR("AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1\r\n")
        ULXR_PCHAR("Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWpr\r\n")
        ULXR_PCHAR("bG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6Ch\r\n")
        ULXR_PCHAR("oqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX\r\n")
        ULXR_PCHAR("2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==\r\n");


     ULXR_COUT << ULXR_PCHAR("Encode 64\n");
     ulxr::CppString s64 = ulxr::encodeBase64(s0);
     ULXR_COUT << s64 << std::endl;

     if (regression != s64)
     {
        ULXR_COUT << ULXR_PCHAR("Encode64 comparison with regression string failed.\n");
        success = false;
     }
     else
       ULXR_COUT << ULXR_PCHAR("Encode64 comparison with regression string OK.\n");

     ULXR_COUT << ULXR_PCHAR("Decode 64\n");
     ulxr::CppString ss = ulxr::decodeBase64(s64);
//     ULXR_COUT << ss << std::endl;

     if (s0 == ss)
       ULXR_COUT << ULXR_PCHAR("base ok\n");
     else
     {
       ULXR_COUT << ULXR_PCHAR("base NOT ok\n");
       success = false;
     }
// ����
     s0 = ULXR_PCHAR("-\r=\t#\n+&<>;12����\"\'contains");
     ulxr::CppString se = ulxr::xmlEscape(s0);

     std::cout << std::hex << "s0: ";
     for (unsigned i1 = 0; i1 < s0.length(); ++i1)
       std::cout << (unsigned) (0xff & s0[i1]) << " ";
     std::cout << std::endl;

     std::cout << std::hex << "se: ";
     for (unsigned i2 = 0; i2 < se.length(); ++i2)
       std::cout << (unsigned) (0xff & se[i2]) << " ";
     std::cout << std::dec << std::endl;

     if (se.find(ULXR_PCHAR("contains")) == ulxr::CppString::npos)
     {
       ULXR_COUT << ULXR_PCHAR("xmlEscape NOT ok\n");
       success = false;
     }
     else
       ULXR_COUT << ULXR_PCHAR("xmlEscape ok\n");

     ULXR_COUT << ULXR_PCHAR("Original: ") << s0 << std::endl;
     ULXR_COUT << ULXR_PCHAR("Escaped: ") << se << std::endl;

     if (s0 == ulxr::xmlUnEscape(se))
       ULXR_COUT << ULXR_PCHAR("escape ok\n");
     else
     {
       ULXR_COUT << ULXR_PCHAR("escape NOT ok\n");
       success = false;
     }

     {
       check_malformed(ULXR_PCHAR("abcd&"));
       check_malformed(ULXR_PCHAR("abcd&#x12"));
       check_malformed(ULXR_PCHAR("abcd&#12"));
       check_malformed(ULXR_PCHAR("abcd&#123456789"));
       check_malformed(ULXR_PCHAR("abcd&#x123456789"));
       check_malformed(ULXR_PCHAR("abcd&#123456789;"));
       check_malformed(ULXR_PCHAR("abcd&#x123456789;"));
     }

#if defined(HAVE_ICONV_H) || defined(HAVE_ICONV)

     ulxr::Cpp8BitString s_lat1 = "������{}[]/'����������{}[]/'����";
     ulxr::Cpp8BitString s02 = ulxr::encodingToUtf8(s_lat1, "LATIN1");
     ULXR_COUT << ULXR_PCHAR("s02: ") << ULXR_GET_STRING(s02) << std::endl;
     ULXR_COUT << ULXR_PCHAR("s_lat1: ") << ULXR_GET_STRING(s_lat1) << std::endl;
     ULXR_COUT << ULXR_PCHAR("back to orig: ")
               << ULXR_GET_STRING(ulxr::utf8ToEncoding(s02, "LATIN1"))
               << std::endl;
     if (s_lat1 == ulxr::utf8ToEncoding(s02, "LATIN1"))
       ULXR_COUT << ULXR_PCHAR("encoding ok\n");
     else
       ULXR_COUT << ULXR_PCHAR("encoding NOT ok\n");

#endif

#ifdef ULXR_UNICODE
     ulxr::Cpp16BitString ws0;
     ws0.reserve(0xd800);
     for (unsigned i = 0x00; i < 0xd800; ++i)
       ws0 += i;

     ulxr::RpcString str (ws0);
     ss = str.getString();
     ULXR_COUT << ss.length() << std::endl;

     ulxr::Cpp16BitString wss = str.getUnicodeString();

     ULXR_COUT << ws0.length() << std::endl;
     ULXR_COUT << (int) ws0[0] << std::endl;

     ULXR_COUT << wss.length() << std::endl;
     ULXR_COUT << (int) wss[0] << std::endl;

     if (ws0 == wss)
       ULXR_COUT << ULXR_PCHAR("unicode 1 ok\n");
     else
     {
       ULXR_COUT << ULXR_PCHAR("unicode 1 NOT ok\n");
       success = false;
     }

     {
        ulxr::Cpp8BitString utf8 = ulxr::unicodeToUtf8(ws0);
        wss = ulxr::utf8ToUnicode(utf8);

        if (ws0 == wss)
          ULXR_COUT << ULXR_PCHAR("unicodeToUtf8 ok\n");
        else
        {
          ULXR_COUT << ULXR_PCHAR("utf8ToUnicode NOT ok\n");
          success = false;
        }
     }
#endif

     {
        ulxr::Cpp8BitString s8;
        s8.reserve(0x100);
        for (unsigned i = 0x00; i < 0x100; ++i)
          s8 += i;

        ulxr::Cpp8BitString utf8 = ulxr::asciiToUtf8(s8);
        ulxr::Cpp8BitString s8s = ulxr::utf8ToAscii(utf8);

        if (s8 == s8s)
          ULXR_COUT << ULXR_PCHAR("asciiToUtf8 ok\n");
        else
        {
          ULXR_COUT << ULXR_PCHAR("utf8ToAscii NOT ok\n");
          success = false;
        }
     }

#ifdef ULXR_UNICODE

     ws0 = ULXR_PCHAR("khera>&sd< asd<f>e56&&& \"$����")
           ULXR_PCHAR("\"\"\'&<&>sd w45234908 0xd9f0�)/()/")
           ULXR_PCHAR("()N u6we4\"E!\"%&((>YXC>YXASESGYDX>YVBHJE%W&TW$");

     ulxr::RpcString str2 (ws0);
     ss = str2.getString();
     wss = str2.getUnicodeString();

     if (ws0 == wss)
       ULXR_COUT << ULXR_PCHAR("unicode ok\n");
     else
     {
       ULXR_COUT << ULXR_PCHAR("unicode NOT ok\n");
       success = false;
     }

#endif

     ULXR_COUT << ULXR_PCHAR("Ready.\n");
  }
  catch(ulxr::Exception &ex)
  {
     ULXR_COUT << ULXR_PCHAR("Error occured: ")
               << ULXR_GET_STRING(ex.why()) << std::endl;
     success = false;
  }

  ULXR_COUT << ULXR_PCHAR("Terminating.\n");
  return success ? 0 : 1;
}
