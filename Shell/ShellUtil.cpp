
#pragma once

#include "Common.h"
#include "ShellException.cpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using std::vector;

class ShellUtil {
public:
	static ShellUtil& getUtilObj() {
		static ShellUtil shellInterfaceUtil;
		return shellInterfaceUtil;
	}

	ShellCommand parseScript(const string input) {
		// 1�� ���� �˻�
		if (input == "1_FullWriteAndReadCompare" || input.rfind("1_", 0) == 0) {
			return SCRIPT_1;
		}
		// 2�� ���� �˻�
		if (input == "2_PartialLBAWrite" || input.rfind("2_", 0) == 0) {
			return SCRIPT_2;
		}
		// 3�� ���� �˻�
		if (input == "3_WriteReadAging" || input.rfind("3_", 0) == 0) {
			return SCRIPT_3;
		}

		return UNKOWN;  // ��Ī�Ǵ� ������ ���� ���
	}

	ShellCommand parse(string commandArg) {
		if (commandArg.compare("read") == 0) {
			return READ_COMMAND;
		}
		if (commandArg.compare("write") == 0) {
			return WRITE_COMMAND;
		}
		if (commandArg.compare("help") == 0) {
			return HELP_COMMAND;
		}
		if (commandArg.compare("exit") == 0) {
			return EXIT_COMMAND;
		}
		if (commandArg.compare("fullwrite") == 0) {
			return FULLWRITE_COMMAND;
		}
		if (commandArg.compare("fullread") == 0) {
			return FULLREAD_COMMAND;
		}
		return UNKOWN;
	}

	// LBA ���ڿ� ��ȯ (10����, 0~99)
	unsigned int convertDecimalStringForLba(const std::string& input) {
		size_t pos;
		unsigned int value = std::stoi(input, &pos, 10);

		// ��ȯ�� ���� Ȯ�� (��: "12abc" ����)
		if (pos != input.length()) {
			throw ShellArgConvertException("Invalid characters in input: " + input);
		}

		// ���� �˻�
		if (value < 0 || value > 99) {
			throw ShellArgConvertException("Value out of range (0-99): " + input);
		}

		return value;
	}

	// Data ���ڿ� ��ȯ (16����, "0x" + 8�ڸ�)
	unsigned int convertHexStringForData(const std::string& input) {
		// ���� üũ
		if (input.length() != 10 || input.substr(0, 2) != "0x") {
			throw ShellArgConvertException("Invalid hex format: " + input);
		}

		size_t pos;
		unsigned int value = std::stoul(input, &pos, 16);

		// ��ȯ�� ���� Ȯ�� (��: "0x1234567G" ����)
		if (pos != input.length()) {
			throw ShellArgConvertException("Invalid characters in input: " + input);
		}

		return value;
	}

	vector<std::string> splitString(const std::string& s) {
		std::vector<std::string> result;
		std::istringstream iss(s);
		std::string word;

		while (iss >> word) { // ����� ���� �ڵ����� ����
			result.push_back(word);
		}

		return result;
	}

	string toTwoDigitString(unsigned int value) {
		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << value;
		return oss.str();
	}

	string toHexFormat(unsigned int value) {
		std::ostringstream oss;
		oss << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << value;
		return oss.str();
	}
private:
	ShellUtil() {

	}
};