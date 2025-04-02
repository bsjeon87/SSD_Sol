#pragma once

#include "Common.h"
#include "ShellCommandInterface.h"
#include "ShellReadCommand.cpp"
#include "ShellWriteCommand.cpp"
#include "ShellScript1.cpp"
#include <memory>

using std::shared_ptr;

class ShellCommandFactory {
public:
	void setDriverInterface(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	shared_ptr<ShellCommandInterface> getCommand(ShellCommand cmd) {
		switch (cmd) {
			case READ_COMMAND:
				return std::make_shared<ShellReadCommand>(mpDriverInterface);
			case WRITE_COMMAND:
				return std::make_shared<ShellWriteCommand>(mpDriverInterface);
			case SCRIPT_1:
				return std::make_shared<ShellScript1>(mpDriverInterface);
		}
		return nullptr;
	}
private:
	SsdDriverInterface* mpDriverInterface;
};