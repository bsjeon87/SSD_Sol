#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"

class ShellFlushCommand : public ShellCommandInterface {
   public:
    ShellFlushCommand();
    string execute(vector<string> args);

   private:
};