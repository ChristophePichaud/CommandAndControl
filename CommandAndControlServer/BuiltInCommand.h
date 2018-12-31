#pragma once
class BuiltInCommand
{
public:
	BuiltInCommand();
	~BuiltInCommand();

public:
	static void CopyFileCmd(std::string source, std::string destination);
	static void MoveFileCmd(std::string source, std::string destination);
	static void DeleteFileCmd(std::string source);
};

