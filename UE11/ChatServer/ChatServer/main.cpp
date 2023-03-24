
#include <WinSock2.h>
#include <process.h>
#include <list>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

struct FUserInfo
{
	SOCKET	Socket;
	SOCKADDR_IN	Addr;
	HANDLE	Thread;
};

std::list<FUserInfo*>	g_User;

unsigned int __stdcall ThreadFunc(void* Arg)
{
	SOCKET	Socket = (SOCKET)Arg;

	while (true)
	{
		char	Packet[2048] = {};

		int Length = recv(Socket, Packet, 2048, 0);

		if (Length <= 0)
			break;

		std::cout << Socket << " Client : " << Packet << "\n";

		auto	iter = g_User.begin();
		auto	iterEnd = g_User.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->Socket == Socket)
				continue;

			send((*iter)->Socket, Packet, Length, 0);
		}
	}

	return 0;
}

int main()
{
	WSADATA	data;

	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN	Addr = {};

	Addr.sin_family = PF_INET;
	Addr.sin_port = htons(7070);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(Socket, (SOCKADDR*)&Addr, sizeof(Addr));

	listen(Socket, 100);

	while (true)
	{
		SOCKADDR_IN	ClntAddr = {};
		int	AddrLength = sizeof(ClntAddr);
		SOCKET	ClntSocket = accept(Socket, (SOCKADDR*)&ClntAddr, &AddrLength);

		std::cout << ClntSocket << " Client Connect\n";

		FUserInfo* User = new FUserInfo;

		User->Socket = ClntSocket;
		User->Addr = ClntAddr;

		g_User.push_back(User);
		

		User->Thread = (HANDLE)_beginthreadex(0, 0, ThreadFunc,
			(void*)ClntSocket, 0, 0);
	}

	closesocket(Socket);

	WSACleanup();

	return 0;
}