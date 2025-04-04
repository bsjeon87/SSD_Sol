#pragma once
#include <iostream>
#include "SSD.cpp"
#include "BufferCommand.h"
#include "command_parser.h"
#include "BufferCommand.h"

using namespace std;


CmdInfo commandParse(int argc, char* argv[])
{
	CommandParser parser;
	CmdInfo ErrorCmd;

	char CMD = *argv[1];  // argv[1]�� �����ϴ� ��쿡�� ����
	std::string LBAstring = (argc >= 3) ? std::string(argv[2]) : "";  // ���� ��� �� ���ڿ�
	char* data = (argc >= 4) ? argv[3] : nullptr;  // ���� ��� nullptr

	//return parser.parseArg(argc, *argv[1], argv[2], argv[3]);
	return parser.parseArg(argc, CMD, LBAstring, data);
	ErrorCmd.IsValid = false;
	return ErrorCmd;
}
void executeCMD(CmdInfo Command)
{
	CommandFileSystem fs;
	BufferCommand CMDBuf(fs);
	CMDBuf.PushCommand(Command);
}


#ifndef UNIT_TEST
int main(int argc, char* argv[]) {
	
	CmdInfo Command = commandParse(argc, argv);
	if (!Command.IsValid) {
		return 0;
	}
	executeCMD(Command);
	return 0;
}
#endif
