#include <iostream>
#include <Windows.h>
#include<tchar.h>
using namespace std;

//���ھ��
HWND mainHWD;
//����ID
DWORD mainPID;
//���̾��
HANDLE mainPROCESS;
//����ַ
DWORD BASE_ADDRESS = 0x007794F8;
//---------����-------------

//һ��ƫ�Ƶ�ַ
DWORD SUN_OFFSET_ONE = 0x868;
//���������ַ
DWORD SUN_OFFSET_TWO = 0x5578;

//---------��ȴʱ��----------

//һ��ƫ�Ƶ�ַ
DWORD CD_OFFSET_ONE = 0x868;
//����ƫ�Ƶ�ַ
DWORD CD_OFFSET_TWO = 0x15c;
//����ƫ�Ƶ�ַ
DWORD CD_OFFSET_THREE = 0x50;
//ֲ���������ƫ�Ƶ�ַ
DWORD KIND_OFFSET_THREE = 0x24;



bool find_window_and_get_pid(const char* window_name)
{
	mainHWD = FindWindow(NULL, _T(window_name));
	if (mainHWD != 0)
	{
		GetWindowThreadProcessId(mainHWD, &mainPID);
		mainPROCESS = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mainPID);
		if (mainPROCESS == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else return false;
}

//��ȴ��0
bool no_cd()
{
	DWORD temp_address;
	int kind_num;

	DWORD VALUE = 0;
	//�ӻ���ַ�ж�ȡ���ݷ���temp_address����������һ��ָ�� 
	ReadProcessMemory(mainPROCESS, (LPVOID)BASE_ADDRESS, &temp_address, 4, 0);

	//�Ӹղŵõ���ָ��+һ��ƫ�ƣ���ȡ���ݣ���������Ȼ��һ��ָ�룬����temp_address
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + CD_OFFSET_ONE), &temp_address, 4, 0);

	//�Ӹղŵõ���ָ��+һ��ƫ�ƣ���ȡ���ݣ���������Ȼ��һ��ָ�룬����temp_address
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + CD_OFFSET_TWO), &temp_address, 4, 0);

	//��ȡֲ����������
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + KIND_OFFSET_THREE), &kind_num, 4, 0);
	//cout << kind_num << endl;
	DWORD now_off = 0x00;
	DWORD res;
	//ѭ���޸�ÿ��ֲ����ȴʱ�� , �����Ҫ������ȴʱ�䣬������һ���̲߳����޸ļ��ɣ�����Ͳ���.
	//ÿ��ֲ������һ���ڴ�ռ䣬����Ϊ0x50

	for (int i = 0; i < kind_num; i++) {
		//cout << now_off << endl;
		res = WriteProcessMemory(mainPROCESS, (LPVOID)(temp_address + CD_OFFSET_THREE + now_off), &VALUE, 4, 0);
		if (res != 0)
		{
			now_off += 0x50;
			continue;
		}
		else {
			return false;
		}
		
	}
	//DWORD res = WriteProcessMemory(mainPROCESS, (LPVOID)(temp_address + CD_OFFSET_THREE + 0x50 ), &VALUE, 4, 0);

	if (res != 0)
	{
		return true;
	}
	else {
		return false;
	}
}


//�޸�����
bool change_sun(int value)
{
	DWORD temp_address;
	DWORD VALUE = (DWORD)(value);
	//�ӻ���ַ�ж�ȡ���ݷ���temp_address����������һ��ָ�� 
	ReadProcessMemory(mainPROCESS, (LPVOID)BASE_ADDRESS, &temp_address, 4, 0);

	//�Ӹղŵõ���ָ��+һ��ƫ�ƣ���ȡ���ݣ���������Ȼ��һ��ָ�룬����temp_address
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + SUN_OFFSET_ONE), &temp_address, 4, 0);

	////�Ӹղŵõ���ָ��+һ��ƫ�ƣ�д�����ݣ������ݱ�ʾ��������
	DWORD res = WriteProcessMemory(mainPROCESS, (LPVOID)(temp_address + SUN_OFFSET_TWO), &VALUE, 4, 0);
	if (res != 0)
	{
		return true;
	}
	else {
		return false;
	}
}



int main()
{
	HWND mainHWD;
	DWORD mainPID;
	bool change_success;
	bool open_success = find_window_and_get_pid("Plants vs. Zombies 1.2.0.1073 RELEASE");
	if (open_success)
	{
		printf("���̴򿪳ɹ�\n");
	}
	else
	{
		printf("���̴�ʧ��");
		return 0;
	}
	while (1)
	{
		//mainHWD = FindWindow(NULL, _T("Plants vs. Zombies 1.2.0.1073 RELEASE"));
		system("cls");
		printf("ѡ���޸����� �� \n1. �޸�����\n2. ����ȴʱ��\n");
		int choice;
		int value;
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("�������޸�����������\n");
			scanf_s("%d", &value);
			change_success = change_sun(value);
			if (change_success) {
				printf("�޸ĳɹ�\n");
			}
			else {
				printf("�޸�ʧ�ܣ�������\n");
			}
			break;
		case 2:
			change_success = no_cd();
			if (change_success) {
				printf("��CD�ɹ�\n");
			}
			else {
				printf("��CDʧ�ܣ�������\n");
			}
			break;
		default:
			break;
		}
		printf("�����������");
		getchar();
		getchar();

	}
	return 0;
}