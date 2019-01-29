#include <iostream>
#include <Windows.h>
#include<tchar.h>
using namespace std;

//窗口句柄
HWND mainHWD;
//进程ID
DWORD mainPID;
//进程句柄
HANDLE mainPROCESS;
//基地址
DWORD BASE_ADDRESS = 0x007794F8;
//---------阳光-------------

//一级偏移地址
DWORD SUN_OFFSET_ONE = 0x868;
//二级编译地址
DWORD SUN_OFFSET_TWO = 0x5578;

//---------冷却时间----------

//一级偏移地址
DWORD CD_OFFSET_ONE = 0x868;
//二级偏移地址
DWORD CD_OFFSET_TWO = 0x15c;
//三级偏移地址
DWORD CD_OFFSET_THREE = 0x50;
//植物格数三级偏移地址
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

//冷却归0
bool no_cd()
{
	DWORD temp_address;
	int kind_num;

	DWORD VALUE = 0;
	//从基地址中读取数据放入temp_address，该数据是一个指针 
	ReadProcessMemory(mainPROCESS, (LPVOID)BASE_ADDRESS, &temp_address, 4, 0);

	//从刚才得到的指针+一个偏移，读取数据，该数据依然是一个指针，放入temp_address
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + CD_OFFSET_ONE), &temp_address, 4, 0);

	//从刚才得到的指针+一个偏移，读取数据，该数据依然是一个指针，放入temp_address
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + CD_OFFSET_TWO), &temp_address, 4, 0);

	//获取植物种类数量
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + KIND_OFFSET_THREE), &kind_num, 4, 0);
	//cout << kind_num << endl;
	DWORD now_off = 0x00;
	DWORD res;
	//循环修改每个植物冷却时间 , 如果需要锁定冷却时间，单独可一个线程不断修改即可，这里就不了.
	//每个植物属于一块内存空间，步长为0x50

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


//修改阳光
bool change_sun(int value)
{
	DWORD temp_address;
	DWORD VALUE = (DWORD)(value);
	//从基地址中读取数据放入temp_address，该数据是一个指针 
	ReadProcessMemory(mainPROCESS, (LPVOID)BASE_ADDRESS, &temp_address, 4, 0);

	//从刚才得到的指针+一个偏移，读取数据，该数据依然是一个指针，放入temp_address
	ReadProcessMemory(mainPROCESS, (LPVOID)(temp_address + SUN_OFFSET_ONE), &temp_address, 4, 0);

	////从刚才得到的指针+一个偏移，写入数据，该数据表示阳光数量
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
		printf("进程打开成功\n");
	}
	else
	{
		printf("进程打开失败");
		return 0;
	}
	while (1)
	{
		//mainHWD = FindWindow(NULL, _T("Plants vs. Zombies 1.2.0.1073 RELEASE"));
		system("cls");
		printf("选择修改内容 ： \n1. 修改阳光\n2. 无冷却时间\n");
		int choice;
		int value;
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("请输入修改阳光数量：\n");
			scanf_s("%d", &value);
			change_success = change_sun(value);
			if (change_success) {
				printf("修改成功\n");
			}
			else {
				printf("修改失败，请重试\n");
			}
			break;
		case 2:
			change_success = no_cd();
			if (change_success) {
				printf("无CD成功\n");
			}
			else {
				printf("无CD失败，请重试\n");
			}
			break;
		default:
			break;
		}
		printf("按任意键继续");
		getchar();
		getchar();

	}
	return 0;
}