
#pragma once

#include "Common.h"
#include "ShellException.cpp"


class ShellInterfaceUtil {
public:
	static ShellInterfaceUtil& getUtilObj() {
		static ShellInterfaceUtil shellInterfaceUtil;
		return shellInterfaceUtil;
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
private:
	ShellInterfaceUtil() {

	}
};