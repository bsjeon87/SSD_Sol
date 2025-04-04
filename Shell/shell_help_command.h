#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "script_store.h"

class ShellHelpCommand : public ShellCommandInterface {
public:
	ShellHelpCommand() {
	}

	string execute(vector<string> args) {
		if (args.size() != 1) {
			throw ShellArgConvertException("args parameter size invalid");
		}
		string output = "Team:Alpha Devs(�̿�ö/�۽�ȣ/�ŵ���/������)\n"
			"READ command: read [LBA]\n"
			"Write command: write [LBA] [DATA:(ex)0x123456]\n"
			"Full Read command: fullread\n"
			"Full Write command: fullwrite\n"
			"Exit command: exit\n";

		output += "\n\n SCRIPT \n\n";
		vector<string>& scriptNames = ScriptStore::getScriptStore().getScriptNames();
		for (string scriptName : scriptNames) {
			output += scriptName + "\n";
		}
		return output;
	}
private:
};