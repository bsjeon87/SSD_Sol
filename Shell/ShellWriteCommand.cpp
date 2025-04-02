#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"

class ShellWriteCommand : public ShellCommandInterface {
public:
	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;
			if (args.size() != 2) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			output.push_back(convertDecimalStringForLba(args[0]));
			output.push_back(convertHexStringForData(args[1]));

			return output;
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}
private:
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
};