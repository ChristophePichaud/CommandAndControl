// CommandAndControlServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

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
	prot.setChunkedTransfer(false);
	prot.setPersistent(persistent);
	ulxr::Dispatcher server(&prot);

	try
	{
		// Add serveur method here
		server.addMethod(ulxr::make_method(ExecuteCmd),
			ulxr::Signature() << ulxr::RpcString(),
			("ExecuteCmd"),
			ulxr::Signature() << ulxr::RpcString(),
			("Execute a command"));

		MyWorker worker;

		server.addMethod(ulxr::make_method(worker, &MyWorker::shutdown),
			ulxr::Signature(ulxr::Boolean()),
			ULXR_PCHAR("testcall_shutdown"),
			ulxr::Signature(),
			ULXR_PCHAR("Testcase with a dynamic method in a class, shut down server, return old state"));


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
