// CCClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <memory>
#include <cstring>
#include <string>

#include <ulxmlrpcpp/ulxr_tcpip_connection.h>  // first, don't move: msvc #include bug
#include <ulxmlrpcpp/ulxr_ssl_connection.h>
#include <ulxmlrpcpp/ulxr_http_protocol.h>
#include <ulxmlrpcpp/ulxr_requester.h>
#include <ulxmlrpcpp/ulxr_value.h>
#include <ulxmlrpcpp/ulxr_except.h>
#include <ulxmlrpcpp/ulxr_log4j.h>

#pragma comment(lib, "msvc71\\x64\\Debug\\ulxmlrpcpp.lib")

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
		std::string host = ("localhost");
		unsigned port = 32000;

		bool big = haveOption(argc, argv, "big");
		bool secure = haveOption(argc, argv, "ssl");
		bool chunked = haveOption(argc, argv, "chunked");
		bool persistent = haveOption(argc, argv, "persistent");

		std::string sec = ("unsecured");

		std::cout << ("Requesting ") << sec << (" rpc calls at ")
			<< host << (":") << port << std::endl
			<< ("Chunked transfer: ") << chunked << std::endl;

		std::unique_ptr<ulxr::TcpIpConnection> conn = std::make_unique<ulxr::TcpIpConnection>(false, host, port);

		ulxr::HttpProtocol prot(conn.get());
		prot.setChunkedTransfer(chunked);
		prot.setPersistent(persistent);

		ulxr::Requester client(&prot);

		ulxr_time_t starttime = ulxr_time(0);
		ulxr::MethodCall executeCmdCall(("ExecuteCmd"));
		std::string str = "notepad.exe";
		executeCmdCall.addParam(ulxr::RpcString(str));

		ulxr::MethodResponse resp;
		std::cout << ("call ExecuteCmd: \n");
		std::cout << "MethodCall: " << executeCmdCall.getXml() << std::endl;
		resp = client.call(executeCmdCall, ("/RPC2"));
		std::cout << "MethodResponse: " << resp.getXml() << std::endl;

		std::cout << ("call testcall_shutdown: \n");
		ulxr::MethodCall testcall_shutdown(("testcall_shutdown"));
		resp = client.call(testcall_shutdown, ("/RPC2"));

		time_t endtime = time(0);
		time_t totalsecs = endtime - starttime;
		time_t mins = totalsecs / 60;
		time_t secs = totalsecs % 60;
		std::cout << ("\nOverall time needed: ")
			<< mins << (":") << secs << std::endl;
	}
	catch (ulxr::Exception &ex)
	{
		std::cout << ("Error occured: ") << ULXR_GET_STRING(ex.why()) << std::endl;
		return 1;
	}

	catch (...)
	{
		std::cout << ("unknown Error occured.\n");
		return 1;
	}

	std::cout << ("Well done, Ready.\n");
	return 0;
}
