#include "ssddriver.h"
#include "config.h"
#include "ShellUtil.cpp"
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
        return false;
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
        //throw
        return ret;
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


std::string SsdDriver::TrimData(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    auto end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

uint32_t SsdDriver::ConvertStrToUint32(const std::string& hexStr) {
    std::string str = TrimData(hexStr);

    // 0x �Ǵ� 0X ����
    if (str.substr(0, 2) == "0x" || str.substr(0, 2) == "0X") {
        str = str.substr(2);
    }

    uint32_t value = 0;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> value;
    return value;
}
