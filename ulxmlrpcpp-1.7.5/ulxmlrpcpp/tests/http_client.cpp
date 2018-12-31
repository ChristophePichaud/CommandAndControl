/***************************************************************************
              http_client.cpp  --  test file for an http client

    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: http_client.cpp 1151 2009-08-12 15:12:01Z ewald-arnold $

 ***************************************************************************/

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <ctime>

#include <ulxmlrpcpp/ulxr_tcpip_connection.h> // first, don't move: msvc #include bug
#include <ulxmlrpcpp/ulxr_http_protocol.h>
#include <ulxmlrpcpp/ulxr_except.h>
#include <ulxmlrpcpp/ulxr_call.h>
#include <ulxmlrpcpp/ulxr_response.h>
#include <ulxmlrpcpp/ulxr_requester.h>
#include <ulxmlrpcpp/ulxr_http_client.h>

#include "util.c"

int main(int argc, char **argv)
{
  ulxr::intializeLog4J(argv[0]);
  ulxr::getLogger4J()->send(ULXR_PCHAR("DEBUG"),
                            ULXR_PCHAR("http_client started"),
                            ULXR_GET_STRING(__FILE__),
                            __LINE__);

  long ret = 0;
  try
  {
     ulxr_FILE *hex = ulxr_fopen("allhex.dat", "wb");
     ulxr::Cpp8BitString hex_compare;
     unsigned  i0;
     unsigned  j;
     for (j = 0; j <= 255; ++j)
       for (i0 = 0; i0 <= 255; ++i0)
       {
         hex_compare += (char) i0;
         ulxr_fputc(i0, hex);
       }
     ulxr_fclose(hex);

     ulxr::CppString host = ULXR_PCHAR("localhost");
     if (argc > 1)
       host = ULXR_GET_STRING(argv[1]);

     unsigned port = 32001;
     if (argc > 2)
       port = ulxr_atoi(argv[2]);

#ifdef __BORLANDC__  // CodeGuard slows down
     unsigned last = 20;
#else
     unsigned last = 500;
#endif
     if (argc > 3)
       last = ulxr_atoi(argv[3]);

     bool wbxml = haveOption(argc, argv, "wbxml");
     bool useproxy = haveOption(argc, argv, "proxy");
     bool useconnect = haveOption(argc, argv, "connect");
     bool chunked = haveOption(argc, argv, "chunked");

     ULXR_COUT << ULXR_PCHAR("WBXML: ") << wbxml << std::endl;
     ULXR_COUT << ULXR_PCHAR("Proxy: ") << useproxy << std::endl;
     ULXR_COUT << ULXR_PCHAR("Proxy-Connect: ") << useconnect << std::endl;
     ULXR_COUT << ULXR_PCHAR("Chunked transfer: ") << chunked << std::endl;

     ulxr::TcpIpConnection conn (false, host, port);
     if (useproxy)
       conn.setProxy(ULXR_PCHAR("sonne.home"), 8080);

     ulxr::HttpProtocol prot(&conn);
     if (useproxy)
       prot.setProxyAuthentication(ULXR_PCHAR("proxyuser"), ULXR_PCHAR("prx!usr"));
     if (useconnect)
       prot.enableConnect();
     prot.setChunkedTransfer(chunked);

     conn.setTimeout(30);  // debugging output needs lots of time
     ulxr::HttpClient http_client(&prot);

     ulxr::Requester rpc_client(&prot, wbxml);

     ulxr::CppString user = ULXR_CHAR("ali-baba");
     ulxr::CppString pass = ULXR_CHAR("open-sesame");
     ulxr::CppString rpc_user = ULXR_CHAR("ali-baba-rpc");
     ulxr::CppString rpc_pass = ULXR_CHAR("open-sesame-rpc");

     conn.open();
     ulxr::Cpp8BitString s;

     ulxr::MethodCall testcall (ULXR_PCHAR("testcall"));

     ulxr_time_t starttime = ulxr_time(0);

     prot.setPersistent(true);
     for (unsigned i1 = 0; i1 < last; ++i1)
     {
       ULXR_COUT << ULXR_CHAR("HTTP Run : ") << i1+1 << std::endl;
       http_client.setMessageAuthentication(user, pass);
       prot.setPersistent(true);

       char buff[100];
       ulxr_sprintf( buff, "%d", i1);
       ulxr::CppString idx = ULXR_GET_STRING(buff);
       ulxr::CppString put_name = ULXR_CHAR("/putmsg.")+idx;
       if (argc >= 2)
         put_name += ULXR_CHAR(".") + ULXR_GET_STRING(argv[1]);

       ulxr::CppString put_message = ULXR_CHAR("hallo put message ")+idx;

       ULXR_COUT << ULXR_CHAR(" == msgGET ") << i1+1 << std::endl;
       s = http_client.msgGET(ULXR_CHAR("/index.html")); // index.html

       ULXR_COUT << ULXR_CHAR(" == msgPUT ") << i1+1 << std::endl;
       http_client.msgPUT(ulxr::getLatin1(put_message), ULXR_CHAR("text/plain"), put_name);

       // minimize race condition when creating file
#ifdef __unix__
       usleep(20*1000);
#elif defined(__WIN32__)
       Sleep(20);
#else
#error unsupported platform here
#endif

       ULXR_COUT << ULXR_CHAR(" == msgGET ") << i1+1 << std::endl;
       s = http_client.msgGET(put_name);

       if (s != ulxr::getLatin1(put_message))
       {
         ULXR_COUT << ULXR_CHAR("Ret:  ") << s.length() << ULXR_CHAR(" ") << ULXR_GET_STRING(s) << std::endl;
         ULXR_COUT << ULXR_CHAR("Sent: ") << put_message.length() << ULXR_CHAR(" ") << put_message << std::endl;
         throw ulxr::Exception(ulxr::ApplicationError,
                               ULXR_CHAR("Error in transmission of msgPUT/msgGET"));
       }

//       POST ist tested with cml-rpc
//       ULXR_COUT << ULXR_CHAR(" == msgPOST ") << i1+1 << std::endl;
//       http_client.msgPOST(ulxr::getLatin1(put_message), ULXR_CHAR("text/plain"), put_name);

       // minimize race condition when creating file
#ifdef __unix__
       usleep(50*1000);
#elif defined(__WIN32__)
       Sleep(100);
#else
#error unsupported platform here
#endif

       ULXR_COUT << ULXR_CHAR(" == doDELETE ") << i1+1 << std::endl;
       http_client.doDELETE(put_name);

       ULXR_COUT << ULXR_CHAR(" == filePUT ") << i1+1 << std::endl;
       http_client.filePUT(ULXR_CHAR("allhex.dat"), ULXR_CHAR("text/plain"), ULXR_CHAR("/allhex.dat.")+idx);

       // minimize race condition when creating file
#ifdef __unix__
       usleep(20*1000);
#elif defined(__WIN32__)
       Sleep(20);
#else
#error unsupported platform here
#endif

       ULXR_COUT << ULXR_CHAR(" == fileGET ") << i1+1 << std::endl;
       http_client.fileGET(ULXR_CHAR("allhex.dat.")+idx, ULXR_CHAR("/allhex.dat.")+idx);

       ulxr_FILE *ifs = ulxr_fopen (ulxr::getLatin1(ULXR_CHAR("allhex.dat.")+idx).c_str(), "rb");
       char buffer [70000];
       unsigned readed = ulxr_fread(buffer, 1, sizeof(buffer), ifs);
       ulxr_fclose(ifs);
       ulxr::Cpp8BitString hex_current;
       hex_current.append(buffer, readed);

       if (hex_compare != hex_current)
       {
         ULXR_COUT << ULXR_CHAR("Ret:  ") << hex_current.length() << std::endl;
         ULXR_COUT << ULXR_CHAR("Sent: ") << hex_compare.length() << std::endl;
         throw ulxr::Exception(ulxr::ApplicationError,
                               ULXR_CHAR("Error in transmission of allhex.dat filePUT/fileGET"));
       }

       if (i1 >= last-1 || (i1 % 2 == 0))
       {
         ULXR_COUT << ULXR_CHAR("not persistent:  ");
         prot.setPersistent(false);
       }
       else
       {
         ULXR_COUT << ULXR_CHAR("persistent:  ");
         prot.setPersistent(true);
       }
       ULXR_COUT << ULXR_CHAR(" == doDELETE ") << i1+1 << std::endl;
       http_client.doDELETE(ULXR_CHAR("/allhex.dat.")+idx);
       ::unlink(ulxr::getLatin1(ULXR_CHAR("allhex.dat.")+idx).c_str());
     }

     for (unsigned i2 = 0; i2 < last; ++i2)
     {
       ULXR_COUT << ULXR_CHAR("RPC Run : ") << i2+1 << std::endl;
       testcall.setParam(ulxr::Integer(45678))
               .addParam(ulxr::Integer(i2));

       ulxr::MethodResponse resp = rpc_client.call(testcall, ULXR_PCHAR("/RPC2"), rpc_user, rpc_pass);

       if (!resp.isOK() )
       {
         ULXR_COUT << ULXR_PCHAR("bad call result:\n");
         ULXR_COUT << resp.getXml(0);
         ++ret;
       }
       else
       {
          ulxr::Struct st = resp.getResult();
          ulxr::Integer sum = st.getMember(ULXR_PCHAR("sum"));
          ulxr::Integer diff = st.getMember(ULXR_PCHAR("difference"));

          if ((unsigned) sum.getInteger() != 45678+i2)
          {
            ULXR_COUT << ULXR_PCHAR("bad sum in result:\n");
            ULXR_COUT << resp.getXml(0);
            ++ret;
          }

          if ((unsigned) diff.getInteger() != 45678-i2)
          {
            ULXR_COUT << ULXR_PCHAR("bad difference in result:\n");
            ULXR_COUT << resp.getXml(0);
            ++ret;
          }
       }
     }

     ulxr_time_t endtime = ulxr_time(0);
     ulxr_time_t mins = (endtime - starttime) / 60;
     ulxr_time_t secs = (endtime - starttime) % 60;
     ULXR_COUT << ULXR_PCHAR("Time needed: ") << mins << ULXR_PCHAR(":")
               << secs << std::endl;

     ulxr::MethodCall number (ULXR_PCHAR("numthreads"));
     ulxr::MethodResponse resp = rpc_client.call(number, ULXR_PCHAR("/RPC2"), rpc_user, rpc_pass);
     ulxr::Integer num = resp.getResult();
     ULXR_COUT << ULXR_PCHAR("Shutting ") << num.getInteger() << ULXR_PCHAR(" servers down..\n");
     ulxr::MethodCall shutter (ULXR_PCHAR("shutdown"));

     for (unsigned i3 = 0; i3 < (unsigned) num.getInteger(); ++i3)
     {
       try
       {
          resp = rpc_client.call(shutter, ULXR_PCHAR("/RPC2"), rpc_user, rpc_pass);
          if (!resp.isOK() )
          {
            ULXR_COUT << ULXR_PCHAR("bad call result:\n");
            ULXR_COUT << resp.getXml(0);
            ++ret;
          }
       }

       catch(ulxr::Exception &ex)
       {
         ULXR_COUT << ULXR_PCHAR("Unexpected problem while transmit()ing shutdown request " << i3 << ".\n")
                   << ex.why() << std::endl;
         ++ret;
         break;
       }

       catch(...)
       {
         ULXR_COUT << ULXR_PCHAR("Unexpected problem while transmit()ing shutdown request " << i3 << ".\n");
         ++ret;
         break;
       }
     }
  }

  catch(ulxr::Exception &ex)
  {
     ULXR_COUT << ULXR_CHAR("Error occured: ") << ex.why() << std::endl;
     ++ret;
  }

  catch(...)
  {
     ULXR_COUT << ULXR_CHAR("unknown Error occured.\n");
     ++ret;
  }

  if (!ret)
    ULXR_COUT << ULXR_CHAR("Ready and ok.\n");
  else
    ULXR_COUT << ULXR_CHAR("Ready but not ok, errors: ") << ret << std::endl;

  return ret;
}
