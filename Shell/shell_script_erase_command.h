#pragma once
#include <memory>

#include "script_parameter_generator.h"
#include "shell_script_command_interface.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

using std::shared_ptr;

class ShellScriptEraseCommand : public ShellScriptCommandInterface {
   public:
        ShellScriptEraseCommand(SsdDriverInterface* pDriverInterface,
                                shared_ptr<ShellScriptParameterGenInterface> startLbaParam,
                                shared_ptr<ShellScriptParameterGenInterface> endLbaParam);
        bool execute();

   private:
        SsdDriverInterface* mpDriverInterface;
        shared_ptr<ShellScriptParameterGenInterface> mStartLbaParam;
        shared_ptr<ShellScriptParameterGenInterface> mEndValueParam;
};
