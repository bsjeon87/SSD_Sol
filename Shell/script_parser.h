
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include "shell_script_command_interface.h"
#include "shell_script_loop_command.h"
#include "shell_script_compare_command.h"
#include "shell_script_write_command.h"
#include "shell_util.h"
#include "script_parameter_generator.h"
using std::shared_ptr;
using std::make_shared;

class ScriptParser {
public:
    void setSsdDriver(SsdDriverInterface* pdriverInterface);
    std::vector<shared_ptr<ShellScriptCommandInterface>> makingScript(string scriptName);
private:

    // ���� ���� �Լ�
    std::string trim(const std::string& s);

    // �Ķ���� ��ū�� �Ľ��ϴ� �Լ�
    shared_ptr< ShellScriptParameterGenInterface> parseParameter(const std::string& token,
        shared_ptr< ShellScriptLoopIdxGetter> looper);

    // ��������� ��ũ��Ʈ ������ �Ľ��ϴ� �Լ�
    // idx�� ���� ���� �ε����� ����Ű��, block ����('}')�� ������ ��ȯ�մϴ�.
    std::vector<shared_ptr<ShellScriptCommandInterface>> parseStatements(shared_ptr<ShellScriptLoopIdxGetter> parentLooper, 
        const std::vector<std::string>& lines, size_t& idx);

    SsdDriverInterface* mpDriverInterface;
};

//���߷���, GenerateLBA, GenerateData Ŭ���� ���