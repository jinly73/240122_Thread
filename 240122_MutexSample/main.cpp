#include <Windows.h>
#include <iostream>

int main()
{
	HANDLE OnceMutex = CreateMutex(NULL, TRUE, L"Hello World");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(nullptr, L"�ѹ��� ����", L"���", MB_OK);
		return 0;
	}
	while (true);


	CloseHandle(OnceMutex);

	return 0;
}