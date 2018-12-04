// CCClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <memory>
#include <cstring>

#include <ulxmlrpcpp/ulxr_tcpip_connection.h>  // first, don't move: msvc #include bug
#include <ulxmlrpcpp/ulxr_ssl_connection.h>
#include <ulxmlrpcpp/ulxr_http_protocol.h>
#include <ulxmlrpcpp/ulxr_requester.h>
#include <ulxmlrpcpp/ulxr_value.h>
#include <ulxmlrpcpp/ulxr_except.h>
#include <ulxmlrpcpp/ulxr_log4j.h>

#pragma comment(lib, "msvc71\\ulxmlrpcpp\\Debug\\ulxmlrpcpp.lib")

bool haveOption(int argc, char **argv, const char *name)
{
	for (int i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], name) == 0)
			return true;
	}
	return false;
}

int main(int argc, char **argv)
{
	try
	{
		ulxr::CppString host = ULXR_PCHAR("localhost");
		unsigned port = 32000;

		bool big = haveOption(argc, argv, "big");
		bool secure = haveOption(argc, argv, "ssl");
		bool chunked = haveOption(argc, argv, "chunked");
		bool persistent = haveOption(argc, argv, "persistent");

		ulxr::CppString sec = ULXR_PCHAR("unsecured");

		ULXR_COUT << ULXR_PCHAR("Requesting ") << sec << ULXR_PCHAR(" rpc calls at ")
			<< host << ULXR_PCHAR(":") << port << std::endl
			<< ULXR_PCHAR("Chunked transfer: ") << chunked << std::endl;

		std::auto_ptr<ulxr::TcpIpConnection> conn;
		std::auto_ptr<ulxr::TcpIpConnection> temp(new ulxr::TcpIpConnection(false, host, port));
		conn = temp;

		ulxr::HttpProtocol prot(conn.get());
		prot.setChunkedTransfer(chunked);
		prot.setPersistent(persistent);
		ulxr::Requester client(&prot);

		ulxr_time_t starttime = ulxr_time(0);

		ulxr::MethodCall executeCmdCall(ULXR_PCHAR("ExecuteCmd"));
		ulxr::CppString str = "CreateProcess cmd.exe";
		executeCmdCall.addParam(ulxr::RpcString(str));

		ulxr::MethodCall testcall_shutdown(ULXR_PCHAR("testcall_shutdown"));

		ulxr::MethodResponse resp;

		ulxr::CppString ret_str;
		ULXR_COUT << ULXR_PCHAR("call ExecuteCmd: \n");
		resp = client.call(executeCmdCall, ULXR_PCHAR("/RPC2"));
		ret_str = resp.getXml(0);
		ULXR_COUT << ULXR_PCHAR("call result: \n");
		ULXR_COUT << ULXR_PCHAR("call result: [No need to display ")
			<< ret_str
			<< ULXR_PCHAR("\n");

		ULXR_COUT << ULXR_PCHAR("call testcall_shutdown: \n");
		resp = client.call(testcall_shutdown, ULXR_PCHAR("/RPC2"));

		ulxr_time_t endtime = ulxr_time(0);
		ulxr_time_t totalsecs = endtime - starttime;
		ulxr_time_t mins = totalsecs / 60;
		ulxr_time_t secs = totalsecs % 60;
		ULXR_COUT << ULXR_PCHAR("\nOverall time needed: ")
			<< mins << ULXR_PCHAR(":") << secs << std::endl;
	}
	catch (ulxr::Exception &ex)
	{
		ULXR_COUT << ULXR_PCHAR("Error occured: ") << ULXR_GET_STRING(ex.why()) << std::endl;
		return 1;
	}

	catch (...)
	{
		ULXR_COUT << ULXR_PCHAR("unknown Error occured.\n");
		return 1;
	}

	ULXR_COUT << ULXR_PCHAR("Well done, Ready.\n");
	return 0;
}
