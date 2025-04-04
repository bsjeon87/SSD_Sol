
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

    void setSsdDriver(SsdDriverInterface* pdriverInterface) {
        mpDriverInterface = pdriverInterface;
    }
    std::vector<shared_ptr<ShellScriptCommandInterface>> makingScript(string scriptName) {
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
            script = parseStatements(nullptr, lines, idx);
        }
        catch (const std::exception& ex) {
            throw ex;
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


    // �Ķ���� ��ū�� �Ľ��ϴ� �Լ�
    shared_ptr< ShellScriptParameterGenInterface> parseParameter(const std::string& token,
            shared_ptr< ShellScriptLoopIdxGetter> looper) {
        // VAL(����)
        std::regex regexVal(R"(VAL\(\s*(\d+)\s*\))");
        // IND(����) �Ǵ� IND() : ���� ������ �⺻ 0
        std::regex regexInd(R"(IND\(\s*(-?\d+)?\s*\))");
        // RAND(����,����,����)
        std::regex regexRand(R"(RAND\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))");
        // RAND_REF(����,����,����)
        std::regex regexRandRef(R"(RAND_REF\(\s*(\d+)\s*\))");

        std::smatch match;

        if (std::regex_match(token, match, regexVal)) {
            return make_shared<ScriptParamValGen>(std::stoi(match[1]));
        }
        else if (std::regex_match(token, match, regexInd)) {
            if (looper == nullptr) {
                throw std::runtime_error("looper 0");
            }

            return make_shared<ScriptParamIdxGen>(looper, std::stoi(match[1]));
        }
        else if (std::regex_match(token, match, regexRand)) {
            return make_shared<ScriptParamRanGen>(std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]));
        }
        else if (std::regex_match(token, match, regexRandRef)) {
            return make_shared<ShellScriptRandGetGen>(std::stoi(match[1]));
        }
        return nullptr;
    }


   
    // ��������� ��ũ��Ʈ ������ �Ľ��ϴ� �Լ�
    // idx�� ���� ���� �ε����� ����Ű��, block ����('}')�� ������ ��ȯ�մϴ�.
    std::vector<shared_ptr<ShellScriptCommandInterface>> parseStatements(shared_ptr<ShellScriptLoopIdxGetter> parentLooper, const std::vector<std::string>& lines, size_t& idx) {
        std::vector<shared_ptr<ShellScriptCommandInterface>> commandVec;
        std::regex loopRegex(R"(^loop\s+(\S+)\s+(\S+)\s+(\S+)\s*\{$)");  // loop header ����

        for (; idx < lines.size(); ++idx) {
            std::string line = trim(lines[idx]);
            if (line.empty())
                continue;

            // ��� ���Ḧ �˸��� '}'�� ������ �� ����� �Ľ� ����
            if (line == "}") {
                // ����� ���� ����Ǿ����Ƿ� idx�� �� ĭ ������Ű�� ��ȯ
                return commandVec;
            }

            std::smatch match;
            if (std::regex_match(line, match, loopRegex)) {
                // loop ��� ����
                shared_ptr< ShellScriptParameterGenInterface> loopStart = parseParameter(match[1].str(), parentLooper);
                shared_ptr< ShellScriptParameterGenInterface> loopEnd = parseParameter(match[2].str(), parentLooper);
                shared_ptr< ShellScriptParameterGenInterface> loopIncrement = parseParameter(match[3].str(), parentLooper);

                ++idx; // loop ���� �ٺ��� ��� ���� �Ľ� ����

                // ��� ȣ���Ͽ� ���� loop�� ���� ���� �Ľ�
                shared_ptr<ShellScriptLoopCommand> compositCmd = std::make_shared<ShellScriptLoopCommand>(
                    loopStart,
                    loopEnd,
                    loopIncrement
                    );
                std::vector<shared_ptr<ShellScriptCommandInterface>> childrenCmds = parseStatements(compositCmd, lines, idx);

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
                    shared_ptr< ShellScriptParameterGenInterface> lba = parseParameter(separatedStr[1], parentLooper);
                    shared_ptr< ShellScriptParameterGenInterface> expected = parseParameter(separatedStr[2], parentLooper);

                    scriptCmd = std::make_shared<ShellScriptCompareCommand>(mpDriverInterface, lba, expected);
                }
                else if (scriptCmdEnum == WRITE_SCRIPT_COMMAND) {
                    shared_ptr< ShellScriptParameterGenInterface> lba = parseParameter(separatedStr[1], parentLooper);
                    shared_ptr< ShellScriptParameterGenInterface> val = parseParameter(separatedStr[2], parentLooper);

                    scriptCmd = std::make_shared<ShellScriptWriteCommand>(mpDriverInterface, lba, val);
                }
                commandVec.push_back(scriptCmd);
            }
        }
        return commandVec;
    }

    SsdDriverInterface* mpDriverInterface;
};

//���߷���, GenerateLBA, GenerateData Ŭ���� ���