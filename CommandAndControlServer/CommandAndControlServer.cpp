// CommandAndControlServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

  // upon requst you may also use intrinsic types like bool, int, double, char*
  // which expand to their xmlrpc counterparts. Define before #including ulxr_value.h!!
#define ULXR_USE_INTRINSIC_VALUE_TYPES

// upon requst you may also use the following construct
//      << ("second" << i2)
#define ULXR_STRUCT_MEMBER_FROM_NAME_VALUE

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <ulxmlrpcpp/ulxr_tcpip_connection.h>  // first, don't move: msvc #include bug
#include <ulxmlrpcpp/ulxr_ssl_connection.h>
#include <ulxmlrpcpp/ulxr_http_protocol.h>
#include <ulxmlrpcpp/ulxr_except.h>
#include <ulxmlrpcpp/ulxr_signature.h>
#include <ulxmlrpcpp/ulxr_dispatcher.h>
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

ulxr::MethodResponse ExecuteCmd(const ulxr::MethodCall &calldata)
{
	std::cout << "ExecuteCmd" << std::endl;
	ulxr::RpcString cmd = calldata.getParam(0);
	std::string s = cmd.getString();
	
	//
	// Do CreateProcess here...
	//

	std::cout << "Param: " << s << std::endl;
	s += " executed...";
	ulxr::MethodResponse resp;
	resp.setResult(ulxr::RpcString(s));
	return resp;
}

class MyWorker
{
public:

	MyWorker() : running(true)
	{}


	ulxr::MethodResponse shutdown(const ulxr::MethodCall &/*calldata*/)
	{
		std::cout << ("got signal to shut down\n");
		ulxr::MethodResponse resp;
		resp.setResult(ulxr::Boolean(running));
		running = false;
		return resp;
	}

	bool running;
};

int MyThread(int argc, char **argv);

int main(int argc, char **argv)
{
	std::thread t1(MyThread, argc, argv);
	t1.join();
	return 0;
}

int MyThread(int argc, char **argv)
{
	std::string host = ("localhost");
	unsigned port = 32000;

	bool secure = haveOption(argc, argv, "ssl");
	bool chunked = haveOption(argc, argv, "chunked");
	bool persistent = haveOption(argc, argv, "persistent");

	std::string sec = ("unsecured");

	std::cout << ("Serving ") << sec << (" rpc requests at ")
		<< host << (":") << port << std::endl
		<< ("Chunked transfer: ") << chunked << std::endl;

	std::unique_ptr<ulxr::TcpIpConnection> conn = std::make_unique<ulxr::TcpIpConnection>(true, host, port);

	ulxr::HttpProtocol prot(conn.get());
	prot.setChunkedTransfer(chunked);
	prot.setPersistent(persistent);
	ulxr::Dispatcher server(&prot);

	try
	{
		server.addMethod(ulxr::make_method(ExecuteCmd),
			ulxr::Signature() << ulxr::RpcString(),
			("ExecuteCmd"),
			ulxr::Signature() << ulxr::RpcString(),
			("Execute a command"));

		MyWorker worker;
		prot.setAcceptCookies(true);
		while (worker.running)
		{
			ulxr::MethodCall call = server.waitForCall();

			ulxr::MethodResponse resp = server.dispatchCall(call);
			if (!prot.isTransmitOnly())
				server.sendResponse(resp);

			if (!prot.isPersistent())
				prot.close();
		}
	}
	catch (ulxr::Exception& ex)
	{
		std::cout << ("Error occured: ") << ULXR_GET_STRING(ex.why()) << std::endl;
		if (prot.isOpen())
		{
			try
			{
				ulxr::MethodResponse resp(1, ex.why());
				if (!prot.isTransmitOnly())
					server.sendResponse(resp);
			}
			catch (...)
			{
				std::cout << ("error within exception occured\n");
			}
			prot.close();
		}
		return 1;
	}
	std::cout << ("Well done, Ready.\n");
	return 0;
}
