// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

// TODO: add headers that you want to pre-compile here
  // upon requst you may also use intrinsic types like bool, int, double, char*
  // which expand to their xmlrpc counterparts. Define before #including ulxr_value.h!!
#define ULXR_USE_INTRINSIC_VALUE_TYPES

// upon requst you may also use the following construct
//      << ("second" << i2)
#define ULXR_STRUCT_MEMBER_FROM_NAME_VALUE

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header
#include <ulxmlrpcpp/ulxr_tcpip_connection.h>
#include <ulxmlrpcpp/ulxr_ssl_connection.h>
#include <ulxmlrpcpp/ulxr_http_protocol.h>
#include <ulxmlrpcpp/ulxr_except.h>
#include <ulxmlrpcpp/ulxr_signature.h>
#include <ulxmlrpcpp/ulxr_dispatcher.h>
#include <ulxmlrpcpp/ulxr_log4j.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\ulxmlrpcpp-1.7.5.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\ulxmlrpcpp-1.7.5.lib")
#endif

#endif //PCH_H
