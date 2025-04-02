#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"
/*
0 ~4�� LBA���� Write ��ɾ �����Ѵ�.
0 ~4�� LBA���� ReadCompare ����
6 ~9�� LBA���� �ٸ� ������ Write ��ɾ �����Ѵ�.
6 ~9�� LBA���� ReadCompare ����
10 ~14�� LBA���� �ٸ� ������Write ��ɾ �����Ѵ�.
10 ~14�� LBA���� ReadCompare ����

���Ͱ�����Ģ������ü����������Full Write + ReadCompare�� �����Ѵ�
*/

class ShellScript1 : public ShellCommandInterface {
public:
	ShellScript1(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			vector<unsigned int> convertedArgs = convertCmdArgs(args);
			string output = "PASS";

			if (compareTest(0, 4) == false) {
				return "FAIL";
			}
			if (compareTest(6, 9) == false) {
				return "FAIL";
			}
			if (compareTest(10, 14) == false) {
				return "FAIL";
			}

			return output;
		}
		catch (ShellArgConvertException e) {
			return "FAIL";
		}
		catch (exception e) {
			return "FAIL";
		}
	}
private:
	bool compareTest(int startLba, int endLba) {
		for (int i = startLba; i <= endLba; i++) {
			mpDriverInterface->writeSSD((int)i, i);
		}
		for (int i = startLba; i <= endLba; i++) {
			unsigned int result = mpDriverInterface->readSSD((int)i);
			if (result != i) {
				return false;
			}
		}

		return true;
	}

	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;
			if (args.size() != 1) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			return output;
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}

	SsdDriverInterface* mpDriverInterface;
};