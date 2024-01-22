#define FD_SETSIZE  100
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(10880);

	bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));

	listen(ListenSocket, 5);

	fd_set ReadSocketList; // 파일 디스크립터 세트 초기화
	FD_ZERO(&ReadSocketList);
	
	// 여기에서 필요한 작업 수행
	// 예를 들어, 소켓을 생성하고 FD_SET을 사용하여 파일 디스크립터를 세트에 추가

	// FD_SET, FD_CLR, FD_ISSET 등을 사용하여 파일 디스크립터 세트를 조작

	FD_SET(ListenSocket, &ReadSocketList);

	struct timeval TimeOut;

	TimeOut.tv_sec = 0;
	TimeOut.tv_usec = 0;

	fd_set CopySocketLists;
	FD_ZERO(&CopySocketLists);

	while (true)
	{
		CopySocketLists = ReadSocketList;

		//polling
		int EventSocketCount = select(0, &CopySocketLists, nullptr, nullptr, &TimeOut);
		if (EventSocketCount == 0)
		{
			//no event
			//cout << "다했냐?" << endl;
		}
		else if (EventSocketCount < 0)
		{
			//error
		}
		else
		{
			//process
			for (int i = 0; i < (int)ReadSocketList.fd_count; i++)
			{
				if (FD_ISSET(ReadSocketList.fd_array[i], &CopySocketLists))
				{
					if (ReadSocketList.fd_array[i] == ListenSocket)
					{
						SOCKADDR_IN ClientSockAddr;
						memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
						int ClientAddrLenth = sizeof(ClientSockAddr);
						SOCKET NewClientSocket = accept(ReadSocketList.fd_array[i], (SOCKADDR*)&ClientSockAddr, &ClientAddrLenth);
						FD_SET(NewClientSocket, &ReadSocketList);
						cout << "connect client : " << inet_ntoa(ClientSockAddr.sin_addr) << endl;
					}
					else
					{
						//recv
						char Buffer[1024] = { 0, };
						int RecvLength = recv(ReadSocketList.fd_array[i], Buffer, 1024, 0);
						if (RecvLength == 0)
						{
							cout << "disconnect client : " << ReadSocketList.fd_array[i] << endl;
							FD_CLR(ReadSocketList.fd_array[i], &ReadSocketList);
						}
						else if (RecvLength < 0)
						{
							cout << "error disconnect client : " << ReadSocketList.fd_array[i] << endl;
							FD_CLR(ReadSocketList.fd_array[i], &ReadSocketList);
						}
						else
						{
							cout << "recv client : " << ReadSocketList.fd_array[i] << " " << Buffer << endl;
							send(ReadSocketList.fd_array[i], Buffer, RecvLength, 0);
						}
					}
				}
			}
		}
	}


	WSACleanup();
	return 0;
}