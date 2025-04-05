#include "shell_util.h"

ShellUtil& ShellUtil::getUtilObj() {
    static ShellUtil shellInterfaceUtil;
    return shellInterfaceUtil;
}

ShellScriptApiCommand ShellUtil::parseScriptApiCmd(const string input) {
    if (input.compare("WRITE") == 0) {
        return WRITE_SCRIPT_COMMAND;
    }
    if (input.compare("COMPARE") == 0) {
        return COMPARE_SCRIPT_COMMAND;
    }
    if (input.compare("ERASE") == 0) {
        return ERASE_SCRIPT_COMMAND;
    }
    return SCRIPT_UNKOWN;
}

ShellCommand ShellUtil::parse(string commandArg) {
    if (commandArg.compare("read") == 0) {
        return SSD_READ_COMMAND;
    }
    if (commandArg.compare("write") == 0) {
        return SSD_WRITE_COMMAND;
    }
    if (commandArg.compare("help") == 0) {
        return SSD_HELP_COMMAND;
    }
    if (commandArg.compare("exit") == 0) {
        return SSD_EXIT_COMMAND;
    }
    if (commandArg.compare("fullwrite") == 0) {
        return SSD_FULLWRITE_COMMAND;
    }
    if (commandArg.compare("fullread") == 0) {
        return SSD_FULLREAD_COMMAND;
    }
    if (commandArg.compare("erase") == 0) {
        return SSD_ERASE_COMMAND;
    }
    if (commandArg.compare("erase_range") == 0) {
        return SSD_ERASERANGE_COMMAND;
    }
    if (commandArg.compare("flush") == 0) {
        return SSD_FLUSH_COMMAND;
    }
    return SSD_SCRIPT_RUN_COMMAND;
}

// LBA ���ڿ� ��ȯ (10����, 0~99)
unsigned int ShellUtil::convertDecimalStringForLba(const std::string& input) {
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

// Size ���ڿ� ��ȯ (10����, 0~100)
unsigned int ShellUtil::convertStrForSize(const std::string& LBA1, const std::string& size) {
    size_t startPos, sizePos;
    unsigned int startLBA = std::stoi(LBA1, &startPos, 10);
    unsigned int sizeLBA = std::stoi(size, &sizePos, 10);
    unsigned int totalSize = startLBA + sizeLBA;

    // ��ȯ�� ���� Ȯ�� (��: "12abc" ����)
    if (sizePos != size.length()) {
        throw ShellArgConvertException("Invalid characters in input: " + size);
    }

    // ���� �˻�
    if (sizeLBA < 0 || sizeLBA > 100) {
        throw ShellArgConvertException("Value out of range (0-100): " + size);
    }

    // LBA + Range�� MAX �ʰ�
    if (totalSize > 100) {
        throw ShellArgConvertException("Erase Range over Max " + size);
    }

    return sizeLBA;
}

unsigned int ShellUtil::convertLBAtoSize(const std::string& LBA1, const std::string& LBA2) {
    size_t pos;
    unsigned int startLBA = std::stoi(LBA1, &pos, 10);
    unsigned int endLBA = std::stoi(LBA2, &pos, 10);

    // ��ȯ�� ���� Ȯ�� (��: "12abc" ����)
    if (pos != LBA2.length()) {
        throw ShellArgConvertException("Invalid characters in input: " + LBA2);
    }

    // ���� �˻�
    if (endLBA < 0 || endLBA > 99) {
        throw ShellArgConvertException("Value out of range (0-99): " + LBA2);
    }

    if (startLBA > endLBA) {
        throw ShellArgConvertException("Start LBA : " + LBA1 + ' ' + "End LBA : " + LBA2);
    }

    unsigned int size = endLBA - startLBA + 1;

    if (startLBA + size > 100) {
        throw ShellArgConvertException("Erase Range over Max " + size);
    }

    return size;
}

// Data ���ڿ� ��ȯ (16����, "0x" + 8�ڸ�)
unsigned int ShellUtil::convertHexStringForData(const std::string& input) {
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

vector<std::string> ShellUtil::splitString(const std::string& s) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string word;

    while (iss >> word) {  // ����� ���� �ڵ����� ����
        result.push_back(word);
    }

    return result;
}

string ShellUtil::toTwoDigitString(unsigned int value) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << value;
    return oss.str();
}

string ShellUtil::toHexFormat(unsigned int value) {
    std::ostringstream oss;
    oss << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << value;
    return oss.str();
}

ShellUtil::ShellUtil() {}