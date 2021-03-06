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

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <memory>
#include <cstring>
#include <string>

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header
#include <ulxmlrpcpp/ulxr_tcpip_connection.h>
#include <ulxmlrpcpp/ulxr_ssl_connection.h>
#include <ulxmlrpcpp/ulxr_http_protocol.h>
#include <ulxmlrpcpp/ulxr_requester.h>
#include <ulxmlrpcpp/ulxr_value.h>
#include <ulxmlrpcpp/ulxr_except.h>
#include <ulxmlrpcpp/ulxr_log4j.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\ulxmlrpcpp-1.7.5.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\ulxmlrpcpp-1.7.5.lib")
#endif

#endif //PCH_H
