#pragma once
#include "ShellCommandFactory.cpp"
#include "ShellInterfaceUtil.cpp"
#include "ssddriver_interface.h"
#include <sstream>

class ShellInterface {
public:
	void setDriverInterface(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
		mCommandFactory.setDriverInterface(pDriverInterface);
	}

	string execute(string input) {

		try {
			vector<string> separatedStr = splitString(input);

			ShellCommand cmd = ShellInterfaceUtil::getUtilObj().parse(separatedStr[0]);
			shared_ptr<ShellCommandInterface> commandExecuter = mCommandFactory.getCommand(cmd);

			return commandExecuter->execute(separatedStr);
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}

private:
	std::vector<std::string> splitString(const std::string& s) {
		std::vector<std::string> result;
		std::istringstream iss(s);
		std::string word;

		while (iss >> word) { // ����� ���� �ڵ����� ����
			result.push_back(word);
		}

		return result;
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

	SsdDriverInterface* mpDriverInterface;
	ShellCommandFactory mCommandFactory;
};