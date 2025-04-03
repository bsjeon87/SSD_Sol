#pragma once

#include "FileSystem.h"
#include "SSD.cpp"

class BufferCommand {
public:
    BufferCommand(FileSystem& filesystem) : fs(filesystem){
       cmdList.clear();
    }

    CmdInfo extractCMDfromFileName( std::string& file)
    {
        CmdInfo cmd;
        cmd.CMDType = file.at(2);
        cmd.LBA = file.substr(4, 1);
        string temp = file.substr(6);
        strcpy_s(cmd.input_data, temp.c_str());
        return cmd;
    }

    void doFlush(testing::internal::Strings& fileList)
    {
        for (auto file : fileList) {
            CmdInfo input;
            input = extractCMDfromFileName(file);

            ssd.parseArg(input.CMDType, input.LBA, input.input_data);   // validate check
            SSD io;
            io.DoWrite(stoi(input.LBA), input.input_data);
        }
    }

    void updateCmdList(CmdInfo cmdInfo) {
        std::vector<string> fileList = fs.getCmdList();
        std::vector<CmdInfo> cmdList;

        // file �̸����� ���� cmd parsing
        int idx = 0;
        for (auto file : fileList) {
            string name = file.substr(2);
            if (name == "empty") {// ���� ���� cmd�� ���� buffer ��
                string newFileName = to_string(idx) +  "_" +
                    std::string(1, cmdInfo.CMDType) + "_" +
                    cmdInfo.LBA + "_" + 
                    cmdInfo.input_data;

                fs.updateFileName(fileList[idx], newFileName);              
                
                break;
            }     
            else { // ������ �ִ� cmd ���� list��
                CmdInfo command= extractCMDfromFileName(file);
                cmdList.push_back(command);
                // ���� ���� CMD �� read ���ٸ�, ���� cmd list ���� ���� LBA �˻�
                if (cmdInfo.LBA == command.LBA)
                {
                    //return cmdInfo.input_data;   data�� return �Ѵ�. (how)
                }
            }
            idx++;
        }

        // full
        if (idx == 5) {
            doFlush(fileList);
            for (int i = 0; i < 5; i++) {
                string newFileName = to_string(i) + "_" + "empty";

                fs.updateFileName(fileList[i], newFileName);
            }


        }
    }
private:
    FileSystem& fs;
    std::vector<string> cmdList;
    SSDCommand ssd;
    std::unique_ptr<FileManager> FileMgr;

};

