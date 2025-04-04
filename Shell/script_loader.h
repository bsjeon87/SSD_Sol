#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <regex>
#include <exception>
#include "script_parser.h"
#include "ssddriver_interface.h"
#include "script_store.h"

namespace fs = std::filesystem;

using std::vector;
using std::string;
using std::exception;

class ScriptLoader {
public:
    void loadScript(SsdDriverInterface* pDriverInterface) {
        vector<string> fileNames = getScriptFileName();
        ScriptParser scriptParser;
        scriptParser.setSsdDriver(pDriverInterface);

        for (string fileName : fileNames) {
            try {
                std::vector<shared_ptr<ShellScriptCommandInterface>> scriptCmds = scriptParser.makingScript(fileName + ".script");
                ScriptStore::getScriptStore().addScript(fileName, scriptCmds);
            }
            catch (exception e) {
                // skip
            }
        }
    }
private:
    vector<string> getScriptFileName() {
        vector<string> fileNames;
        std::regex validFormat(R"(^\d+_.+$)");

        // ���� ���丮 �� ��� �׸��� ��ȸ�մϴ�.
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            // regular file(�Ϲ� ����)�̰� Ȯ���ڰ� ".script"�� ���
            if (entry.is_regular_file() && entry.path().extension() == ".script") {
                std::string stem = entry.path().stem().string();
                if (std::regex_match(stem, validFormat)) {
                    fileNames.push_back(stem);
                }
            }
        }
        return fileNames;
    }
};