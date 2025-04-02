
#include <gmock/gmock.h>
#include "../File_Interface.cpp"
#include "../SSD.cpp"
using namespace testing;

class SSDTestFixture : public Test
{
public:
	SSDReadDriver SSDRead;
	SSDWriteDriver *SSDWrite;
	FileInterface* SSDIn;
};

class MockFile : public FileInterface {
public:
	MOCK_METHOD(bool, open, (const std::string& filename), ());
};
#if 0
TEST_F(SSDTestFixture, FileOpen) {
	MockFile file;
	EXPECT_CALL(file, open("ssd_nand.txt")).WillRepeatedly(Return(true));

	FileInterface ssdIO;
	EXPECT_EQ(true, ssdIO.open("ssd_nand.txt"));
}

TEST_F(SSDTestFixture, FileOpenCheck) {
	MockFile file;
	EXPECT_CALL(file, open("ssd_nand.txt")).WillRepeatedly(Return(true));

	FileInterface ssdIO;
	EXPECT_EQ(true, ssdIO.isOpen());
}

TEST_F(SSDTestFixture, FileClose) {
	MockFile file;
    EXPECT_CALL(file, open("ssd_nand.txt")).WillRepeatedly(Return(false));

	FileInterface ssdIO;
	ssdIO.close();

	EXPECT_EQ(false, ssdIO.isOpen());
}

// (invalide check) Read Wrie �����
TEST_F(SSDTestFixture, invalidecheck1) {
	// CMD �� W, R �ΰ��� ����
	int value = 0;
	EXPECT_THAT(value, AnyOf(Eq('W'), Eq('R')));
}

TEST_F(SSDTestFixture, invalidecheck2) {
	
	// LBA 0 ~ 100 ������ ����
	int LBA = 0;
	EXPECT_THAT(LBA, Ge(0));
	EXPECT_THAT(LBA, Le(99));
}

TEST_F(SSDTestFixture, invalidecheck3) {
	// LBA 0 ~ 100 �ܴ̿� "ERROR message" 
}

TEST_F(SSDTestFixture, invalidecheck4) {
	//Data �� 0x �����Ͽ� 10��
}
#endif
TEST_F(SSDTestFixture, Read1) {
	//������ ������ 0 ���� return
	
}
TEST_F(SSDTestFixture, Read2) {
	//SSD_output.txt �� ������ file ����

}
TEST_F(SSDTestFixture, Read3) {
//Read ��û�� SSD_Output.txt �� write�ϰ�, read �Ͽ� write ������ ��������
}
TEST_F(SSDTestFixture, Read4) {
// ���� LBA (0~99) ���� ��, LBA �˻��Ͽ� data read
}

TEST_F(SSDTestFixture, ReadAfterWrite) {
	std::string data = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data);

	EXPECT_EQ(data, SSDRead.DoRead(0x0));
}

TEST_F(SSDTestFixture, WriteSameLBA) {
	std::string data1 = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data1);

	std::string data2 = "0x15678";
	SSDWrite.DoWrite(0x1, data2);

	data1 = "0x87654321";
	SSDWrite.DoWrite(0x0, data1);

	data2 = "0x1234321";
	SSDWrite.DoWrite(0x1, data2);

	//EXPECT_EQ(data1, SSDRead.DoRead(0));
	//EXPECT_EQ(data2, SSDRead.DoRead(1));


}


TEST_F(SSDTestFixture, Write2) {
// write data �� LBA, data �� SSD_nand.txt ����
}
TEST_F(SSDTestFixture, Write3) {
// ���ο� write cmd�� ������ data ���� write 
}
TEST_F(SSDTestFixture, Write4) {
// ���� LBA write�� LBA �˻� ��, data ���� (overwrite�̳�, ��ġ�� ����)
}

#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
