
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include "ShellScriptCommandInterface.h"
#include "ShellScriptLoopCommand.cpp"
#include "ShellScriptCompareCommand.cpp"
#include "ShellScriptWriteCommand.cpp"
#include "ShellUtil.cpp"
using std::shared_ptr;

class ScriptParser {
public:

    std::vector<shared_ptr<ShellScriptCommandInterface>> MakingScript(string scriptName) {
        std::vector<shared_ptr<ShellScriptCommandInterface>> script;

        std::ifstream file(scriptName);
        if (!file) {
            throw std::runtime_error("Error: .");
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        try {
            size_t idx = 0;
            script = parseStatements(lines, idx);
        }
        catch (const std::exception& ex) {
            throw std::runtime_error("Error: Unprocessed extra lines remain in the script.");
        }
        return script;
    }
private:

    // ���� ���� �Լ�
    std::string trim(const std::string& s) {
        auto start = s.begin();
        while (start != s.end() && std::isspace(*start)) {
            start++;
        }
        auto end = s.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
        return std::string(start, end + 1);
    }

    // ��������� ��ũ��Ʈ ������ �Ľ��ϴ� �Լ�
    // idx�� ���� ���� �ε����� ����Ű��, block ����('}')�� ������ ��ȯ�մϴ�.
    std::vector<shared_ptr<ShellScriptCommandInterface>> parseStatements(const std::vector<std::string>& lines, size_t& idx) {
        std::vector<shared_ptr<ShellScriptCommandInterface>> commandVec;
        std::regex loopRegex(R"(^loop\s+(\d+)\s*\{\s*$)");  // loop header ����

        for (; idx < lines.size(); ++idx) {
            std::string line = trim(lines[idx]);
            if (line.empty())
                continue;

            // ��� ���Ḧ �˸��� '}'�� ������ �� ����� �Ľ� ����
            if (line == "}") {
                // ����� ���� ����Ǿ����Ƿ� idx�� �� ĭ ������Ű�� ��ȯ
                ++idx;
                return commandVec;
            }

            std::smatch match;
            if (std::regex_match(line, match, loopRegex)) {
                // loop ��� ����
                int loopCnt = std::stoi(match[1]);
                ++idx; // loop ���� �ٺ��� ��� ���� �Ľ� ����

                // ��� ȣ���Ͽ� ���� loop�� ���� ���� �Ľ�
                shared_ptr<ShellScriptLoopCommand> compositCmd = std::make_shared<ShellScriptLoopCommand>(loopCnt);
                std::vector<shared_ptr<ShellScriptCommandInterface>> childrenCmds = parseStatements(lines, idx);

                for (shared_ptr<ShellScriptCommandInterface> scriptCommand : childrenCmds) {
                    compositCmd->addCommand(scriptCommand);
                }
                commandVec.push_back(compositCmd);
            }
            else {
                // loop header�� �ƴ� ���� �ܼ� ��ɾ�� ó�� (�ݴ� �߰�ȣ '}'�� ���⼭ �ɷ���)
                // ���� line�� "loop"�� ���������� ������ �ùٸ��� ������ ���� ó��
                if (line.find("loop") == 0) {
                    throw std::runtime_error("Error: Malformed loop statement or missing '{' at line: " + line);
                }
                // ��ɾ� �Ľ�: �������� �и�
                vector<string> separatedStr = ShellUtil::getUtilObj().splitString(line);
                ShellScriptApiCommand scriptCmdEnum = ShellUtil::getUtilObj().parseScriptApiCmd(separatedStr[0]);
                shared_ptr<ShellScriptCommandInterface> scriptCmd;

                if (scriptCmdEnum == SCRIPT_UNKOWN) {
                    throw std::runtime_error("unkown cmd");
                }
                else if (scriptCmdEnum == COMPARE_SCRIPT_COMMAND) {
                    scriptCmd = std::make_shared<ShellScriptCompareCommand>(nullptr, separatedStr);
                }
                else if (scriptCmdEnum == WRITE_SCRIPT_COMMAND) {
                    scriptCmd = std::make_shared<ShellScriptWriteCommand>(nullptr, separatedStr);
                }
                commandVec.push_back(scriptCmd);
            }
        }

        return commandVec;
    }
};

//���߷���, GenerateLBA, GenerateData Ŭ���� ���