#pragma once

#include "common.h"
#include "shell_command_interface.h"
#include "shell_read_command.h"
#include "shell_write_command.h"
#include "shell_full_read_command.h"
#include "shell_full_write_command.h"
#include "shell_help_command.h"
#include "shell_exit_command.h"
#include "shell_erase_command.h"
#include "shell_erase_range_command.h"
#include "shell_flush_command.h"
#include "shell_script_run_command.h"
#include <memory>

using std::shared_ptr;

class ShellCommandFactory {
public:
	void setDriverInterface(SsdDriverInterface* pDriverInterface);
	shared_ptr<ShellCommandInterface> getCommand(ShellCommand cmd);
private:
	SsdDriverInterface* mpDriverInterface;
};