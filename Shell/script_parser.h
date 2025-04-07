
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "script_parameter_generator.h"
#include "shell_script_command_interface.h"
#include "common.h"
#include <stack>

using std::make_shared;
using std::shared_ptr;

class ScriptParser {
   public:
    std::vector<shared_ptr<ShellScriptCommandInterface>> makingScript(string scriptName);

   private:
    ShellScriptApiCommand parseScriptApiCmd(const string input);
    std::string trim(const std::string& s);
    // �Ķ���� ��ū�� �Ľ��ϴ� �Լ�
    shared_ptr<ShellScriptParameterGenInterface> parseParameter(
        const std::string& token, shared_ptr<ShellScriptLoopIdxGetter> looper);
    // ��������� ��ũ��Ʈ ������ �Ľ�
    std::vector<shared_ptr<ShellScriptCommandInterface>> parseStatements(
        shared_ptr<ShellScriptLoopIdxGetter> parentLooper, const std::vector<std::string>& lines,
        size_t& idx);
    void validationStackClear();
    std::stack<char> mValidationStack;
};
