
#pragma once

#include "common.h"
#include "shell_exception.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using std::vector;

class ShellUtil {
public:
	static ShellUtil& getUtilObj();
	ShellScriptApiCommand parseScriptApiCmd(const string input);
	ShellCommand parse(string commandArg);
	// LBA ���ڿ� ��ȯ (10����, 0~99)
	unsigned int convertDecimalStringForLba(const std::string& input);
	// Size ���ڿ� ��ȯ (10����, 0~100)
	unsigned int convertStrForSize(const std::string& LBA1, const std::string& size);
	unsigned int convertLBAtoSize(const std::string& LBA1, const std::string& LBA2);
	// Data ���ڿ� ��ȯ (16����, "0x" + 8�ڸ�)
	unsigned int convertHexStringForData(const std::string& input);
	vector<std::string> splitString(const std::string& s);
	string toTwoDigitString(unsigned int value);
	string toHexFormat(unsigned int value);
private:
	ShellUtil();
};