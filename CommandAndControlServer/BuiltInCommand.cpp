#include "pch.h"
#include "BuiltInCommand.h"


BuiltInCommand::BuiltInCommand()
{
}


BuiltInCommand::~BuiltInCommand()
{
}

void BuiltInCommand::CopyFileCmd(std::string source, std::string destination)
{
	::CopyFile(source.c_str(), destination.c_str(), FALSE);
}

void BuiltInCommand::MoveFileCmd(std::string source, std::string destination)
{
	::MoveFile(source.c_str(), destination.c_str());
}

void BuiltInCommand::DeleteFileCmd(std::string source)
{
	::DeleteFile(source.c_str());
}

