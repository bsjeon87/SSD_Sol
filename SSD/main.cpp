#pragma once
#include <iostream>

#include "ssd_buffer_command.h"
#include "ssd_command_executor.h"
#include "ssd.cpp"
#include "ssd_command_parser.h"

using namespace std;

CmdInfo commandParse(int argc, char *argv[]) {
  CommandParser parser;
  CmdInfo ErrorCmd;
  
  if (argc < 2) {
    ErrorCmd.IsValid = false;
    return ErrorCmd;
  }

  char CMD = *argv[1];  // argv[1]�� �����ϴ� ��쿡�� ����
  std::string LBAstring =
      (argc >= 3) ? std::string(argv[2]) : "";   // ���� ��� �� ���ڿ�
  char *data = (argc >= 4) ? argv[3] : nullptr;  // ���� ��� nullptr
  return parser.parseArg(argc, CMD, LBAstring, data);
}

void executeCMD(const CmdInfo &command, BufferCommand &bufferCmd) {
  bufferCmd.pushCommand(command);
}
#ifndef UNIT_TEST
int main(int argc, char *argv[]) {
  CmdInfo Command = commandParse(argc, argv);
  if (!Command.IsValid) {
    return 0;
  }
  CommandFileSystem fs;
  BufferCommand bufferCmd(fs);
  executeCMD(Command, bufferCmd);
  return 0;
}
#endif
