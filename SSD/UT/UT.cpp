#include <gmock/gmock.h>

using namespace testing;

TEST(SSD_TCSuite, invalidecheck1) {
	// CMD �� W, R �ΰ��� ����
	int value = 0;
	EXPECT_THAT(value, AnyOf(Eq('W'), Eq('R')));
}

// (invalide check) Read Wrie �����
TEST(SSD_TCSuite, invalidecheck2) {
	
	// LBA 0 ~ 100 ������ ����
	int LBA = 0;
	EXPECT_THAT(LBA, Ge(0));
	EXPECT_THAT(LBA, Le(99));
}

TEST(SSD_TCSuite, invalidecheck3) {
	// LBA 0 ~ 100 �ܴ̿� "ERROR message" 
}

TEST(SSD_TCSuite, invalidecheck4) {
	//Data �� 0x �����Ͽ� 10��
}

TEST(SSD_TCSuite, Read1) {
	//������ ������ 0 ���� return
}
TEST(SSD_TCSuite, Read2) {
	//SSD_output.txt �� ������ file ����

}
TEST(SSD_TCSuite, Read3) {
//Read ��û�� SSD_Output.txt �� write�ϰ�, read �Ͽ� write ������ ��������
}
TEST(SSD_TCSuite, Read4) {
// ���� LBA (0~99) ���� ��, LBA �˻��Ͽ� data read
}

TEST(SSD_TCSuite, Write1) {
// SSD_nand.txt �� ������ file ����
}

TEST(SSD_TCSuite, Write2) {
// write data �� LBA, data �� SSD_nand.txt ����
}
TEST(SSD_TCSuite, Write3) {
// ���ο� write cmd�� ������ data ���� write 
}
TEST(SSD_TCSuite, Write4) {
// ���� LBA write�� LBA �˻� ��, data ���� (overwrite�̳�, ��ġ�� ����)
}


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}