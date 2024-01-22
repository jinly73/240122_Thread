#include <Windows.h>
#include <iostream>

int main()
{
	HANDLE OnceMutex = CreateMutex(NULL, TRUE, L"Hello World");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(nullptr, L"한번만 실행", L"경고", MB_OK);
		return 0;
	}
	while (true);


	CloseHandle(OnceMutex);

	return 0;
}