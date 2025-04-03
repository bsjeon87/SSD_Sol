#include "ssddriver.h"
#include "config.h"
#include "ShellUtil.cpp"
#include "ssddriver_exception.cpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <windows.h>  // Windows API ���

bool SsdDriver::executeCmd(std::string cmdLine) {
    char path[MAX_PATH];  // ���� ���� ��ü ��θ� ������ ����
    GetModuleFileNameA(NULL, path, MAX_PATH);  // ���� ���� ������ ��ü ��� ��������

    std::string exePath = path;  // std::string���� ��ȯ
    size_t pos = exePath.find_last_of("\\");  // ������ '\'�� ��ġ ã��
    std::string dirPath = exePath.substr(0, pos);  // ���� ������ �ִ� ���͸� ���

    // ������ command.exe ��� ����
    std::string newCmd = dirPath + "\\" + cmdLine;

    int result = system(newCmd.c_str());

    if (result) {
        try {
            return false;
        }
        catch (exception e) {
            throw e;
        }
        catch (SsdExecuteException e) {
            throw SsdExecuteException();
        }
    }
    return true;
}

uint32_t SsdDriver::readSSD(int LBA) {
    std::string cmd = "SSD.exe R " + ShellUtil::getUtilObj().toTwoDigitString(LBA);
    const char* cmdLine= cmd.c_str();

    executeCmd(cmdLine);

    uint32_t ret = 0xFFFFFFFF;

    std::ifstream file(config::FILE_PATH); 

    if (!file.is_open())
    {
        try {
            return ret;
        }
        catch (exception e) {
            throw e;
        }
        catch (FileNotFoundException e) {
            throw FileNotFoundException();
        }
    }

    std::string outputData;
    std::getline(file, outputData);
    ret = ShellUtil::getUtilObj().convertHexStringForData(outputData);

    return ret;
}

void SsdDriver::writeSSD(int LBA, uint32_t data) {
    std::string cmd = "SSD.exe W " + ShellUtil::getUtilObj().toTwoDigitString(LBA) + " "
        + ShellUtil::getUtilObj().toHexFormat(data);
    const char* cmdLine = cmd.c_str();

    executeCmd(cmdLine);
}
