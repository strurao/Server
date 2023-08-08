#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include "windows.h"
#include <atomic>
#include <Windows.h>
#include <mutex> 
#include <map>
#include <queue>
using namespace std;
#include "ThreadManager.h"

/*
TCP 서버
1) 새로운 소켓 생성          socket
2) 소켓에 주소/포트 번호설정  bind
3) 소켓 일 시키기           listen
4) 손님 접속                accept
5) 클라와 통신
*/

int main()
{
	SocketUtils::Init();

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	// https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-ioctlsocket
	// 논블로킹 소켓으로 -> 기다리지 않고 빠져나오는 비동기 방식
	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SocketUtils::SetReuseAddress(listenSocket, true);

	if (SocketUtils::BindAnyAddress(listenSocket, 7777) == false)
		return 0;

	if (SocketUtils::Listen(listenSocket, SOMAXCONN) == false)
		return 0;

	SOCKADDR_IN clientAddr;
	int32 addrLen = sizeof(clientAddr);

	// Accept
	while (true)
	{
		// 마냥 대기하지 않고 무조건 빠져나온다는 것이 큰 차이점이다
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		// 아직까지 나한테 접속 요청한 클라이언트가 없다면
		if (clientSocket == INVALID_SOCKET) 
		{
			// 원래라면 블로킹(WSAEWOULDBLOCK) 했어야 했는데... 너가 논블로킹으로 하라며?
			if (::WSAGetLastError() == WSAEWOULDBLOCK) 
				continue; // 진짜 에러는 아니고 아무도 없다는 의미이니까 빠져나오기
		}

		// 접속 요청이 들어왔다면
		cout << "Client Connected!" << endl;

		// Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				// 원래 블로킹했어야 했는데... 너가 논블로킹으로 하라며?
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				// TODO
				break;
			}

			
			cout << "Recv Data = " << recvBuffer << endl;
			cout << "Recv Data len = " << recvLen << endl;
		}
	}

	SocketUtils::Clear();
}