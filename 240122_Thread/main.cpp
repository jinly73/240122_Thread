#include <process.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "winmm")

using namespace std;

int money = 0;
int money1 = -100;


// UserMode
CRITICAL_SECTION MoneySection; // ���� ����
CRITICAL_SECTION MoneySection2;


// Kernel Mode
HANDLE MoneyMutex;

//unsigned(__stdcall* _beginthreadex_proc_type)(void*);


//race condition
unsigned Increase(void* Arg) // ������ ����
{
	EnterCriticalSection(&MoneySection); // In
	for (int i = 0; i < 10000; i++)
	{
		// �Ӱ迵�� ����
		//WaitForSingleObject(MoneyMutex, INFINITE); // �ñ׳�(Bool) ���³İ� ����� �� 
		money = money + 1;
		money1++;
		//ReleaseMutex(MoneyMutex);
	}
	LeaveCriticalSection(&MoneySection); // Out
	return 0;
}

unsigned Decrease(void* Arg)
{
	EnterCriticalSection(&MoneySection);
	for (int i = 0; i < 10000; i++)
	{
		//WaitForSingleObject(MoneyMutex, INFINITE);
		money = money - 1;
		money1--;
		//ReleaseMutex(MoneyMutex);
	}
	LeaveCriticalSection(&MoneySection);

	return 0;
}


// main thread
int main()
{
	//Thread Pooling
	//KernelMode Lock Object
	// UserMode Lock Object
	InitializeCriticalSection(&MoneySection); // �ʱ�ȭ
	InitializeCriticalSection(&MoneySection2);
	
	DWORD StartTime = timeGetTime();
	MoneyMutex = CreateMutex(nullptr, FALSE, nullptr); // �ñ׳λ��� ����
	HANDLE ThreadHandle[2];

	ThreadHandle[0] = (HANDLE)_beginthreadex(nullptr, 0, Increase, nullptr, 0, nullptr);
	ThreadHandle[1] = (HANDLE)_beginthreadex(nullptr, 0, Decrease, nullptr, 0, nullptr);

	ResumeThread(ThreadHandle[0]); // �ٽ� �������ּ���
	ResumeThread(ThreadHandle[1]);

	SuspendThread(ThreadHandle[0]); // 
	SuspendThread(ThreadHandle[1]);

	//Sleep(1000); // Seconde �� �ð� �ڿ� ����
	WaitForMultipleObjects(2, ThreadHandle, TRUE, INFINITE);

	cout << "Process : " << timeGetTime() - StartTime << endl;

	DeleteCriticalSection(&MoneySection); // �ʱ�ȭ

	CloseHandle(MoneyMutex); // KernelMode �ʱ�ȭ
	CloseHandle(ThreadHandle[0]);
	CloseHandle(ThreadHandle[1]);
	

	cout << money << endl;

	return 0;

}