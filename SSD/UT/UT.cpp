#include <gmock/gmock.h>

using namespace testing;

class SSDTestFixture : public Test
{
public:
	
};


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

TEST_F(SSDTestFixture, Write1) {
// SSD_nand.txt �� ������ file ����
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


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}