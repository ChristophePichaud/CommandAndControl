// CCClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

		// ExecuteCmd
		ulxr::MethodCall executeCmdCall(("ExecuteCmd"));
		std::string str = "notepad.exe";
		executeCmdCall.addParam(ulxr::RpcString(str));

		ulxr::MethodResponse resp;
		std::cout << ("call ExecuteCmd: \n");
		std::cout << "MethodCall: " << executeCmdCall.getXml() << std::endl;
		resp = client.call(executeCmdCall, ("/RPC2"));
		std::cout << "MethodResponse: " << resp.getXml() << std::endl;

		// Method1
		ulxr::MethodCall executeCall1(("Method1"));
		std::string param0 = "CopyFile";
		executeCall1.addParam(ulxr::RpcString(param0));
		std::string param1 = "CCClient.exe";
		executeCall1.addParam(ulxr::RpcString(param1));
		//std::string param2 = "CCClient2.exe";
		//executeCall1.addParam(ulxr::RpcString(param2));
		std::cout << ("call ExecuteCmdFile: \n");
		std::cout << "MethodCall: " << executeCall1.getXml() << std::endl;
		ulxr::MethodResponse respMethod1;
		respMethod1 = client.call(executeCall1, ("/RPC2"));
		std::cout << "MethodResponse: " << respMethod1.getXml() << std::endl;

		// CopyFile
		ulxr::MethodCall executeCmdFileCall(("ExecuteCmdFile"));
		std::string strCmd = "CopyFile";
		executeCmdFileCall.addParam(ulxr::RpcString(strCmd));
		std::string str1 = "CCClient.exe";
		executeCmdFileCall.addParam(ulxr::RpcString(str1));
		std::string str2 = "CCClient2.exe";
		executeCmdFileCall.addParam(ulxr::RpcString(str2));
		std::cout << ("call ExecuteCmdFile: \n");
		std::cout << "MethodCall: " << executeCmdFileCall.getXml() << std::endl;
		ulxr::MethodResponse respCopyFile;
		respCopyFile = client.call(executeCmdFileCall, ("/RPC2"));
		std::cout << "MethodResponse: " << respCopyFile.getXml() << std::endl;

		// MoveFile
		ulxr::MethodCall executeCmdFileCall2(("ExecuteCmdFile"));
		strCmd = "MoveFile";
		executeCmdFileCall2.addParam(ulxr::RpcString(strCmd));
		str1 = "CCClient2.exe";
		executeCmdFileCall2.addParam(ulxr::RpcString(str1));
		str2 = "CCClient3.exe";
		executeCmdFileCall2.addParam(ulxr::RpcString(str2));
		std::cout << ("call ExecuteCmdFile: \n");
		std::cout << "MethodCall: " << executeCmdFileCall2.getXml() << std::endl;
		resp = client.call(executeCmdFileCall2, ("/RPC2"));
		std::cout << "MethodResponse: " << resp.getXml() << std::endl;

		// DeleteFile
		ulxr::MethodCall executeCmdFileCall3(("ExecuteCmdFile"));
		strCmd = "DeleteFile";
		executeCmdFileCall3.addParam(ulxr::RpcString(strCmd));
		str1 = "CCClient3.exe";
		executeCmdFileCall3.addParam(ulxr::RpcString(str1));
		str2 = "";
		executeCmdFileCall3.addParam(ulxr::RpcString(str2));
		std::cout << ("call ExecuteCmdFile: \n");
		std::cout << "MethodCall: " << executeCmdFileCall3.getXml() << std::endl;
		resp = client.call(executeCmdFileCall3, ("/RPC2"));
		std::cout << "MethodResponse: " << resp.getXml() << std::endl;

		// Shutdown
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
