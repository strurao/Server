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
4) 손님 접속               accept
5) 클라와 통신
*/

/*
UDP 서버
1) 새로운 소켓 생성 (socket)
2) 소켓에 주소/포트 번호 설정 (bind)
3) 클라와 통신
*/

int main()
{
	// 처음 
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// 1) 소켓 생성, 통신 방식 설정
	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0
	// return : descriptor
	//int32 errorCode = ::WSAGetLastError();
	
	// TCP 문지기 휴대폰
	// SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0); 
	// UDP 문지기 휴대폰
	SOCKET listenSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 

	if (listenSocket == INVALID_SOCKET)
		return 0;

	// 2) 주소/포트 번호 설정 (bind)
	// 연결할 목적지는? (IP주소 + Port) -> XX 아파트 YY 호
	SOCKADDR_IN serverAddr; // 우리 레스토랑의 상호 주소
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); // INADDR_ANY 에 Ipv4 주소인  192.168.XX.X 가 들어간다
	serverAddr.sin_port = ::htons(7777); // 7777port를 개통하겠다! 참고로 80port는 HTTP

	// host to network short
	// Little-Endian vs Big-Endian
	// ex) 0x12345678 4바이트 정수
	// low [0x78][0x56][0x34][0x12] high < little
	// low [0x12][0x34][0x56][0x78] high < big = network 표준

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) // binding 해준다
		return 0;

	// 3) 업무 개시 (listen)
	// if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	//  	return 0;

	// 4) 손님 맞이 (accept)
	while (true)
	{
		SOCKADDR_IN clientAddr; // 상대방 주소
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);

		// 상대방 소켓 부여. 동접5000명이라면 5000개의 clientSocket
		// 손님이 없다면 (상대방 쪽에서 connect 하지 않는다면) accept에서 대기한다. 멈춰있다.
		
		/*
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
			return 0;

		// 손님 입장!
		char ip[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
		cout << "Client Connected! IP = " << ip << endl;
		*/

		// 5) TODO 패킷
		// while (true)
		// {
			// msg 수신하는 가장 기본적인 함수는 recv()
			char recvBuffer[100]; // 커널의 recvBuffer 에 있는 걸 내가 지정한 (유저레벨) 곳에 복사해줘
			
			// TCP
			// int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			// UDP
			int32 recvLen = ::recvfrom(listenSocket, recvBuffer, sizeof(recvBuffer), 0, (SOCKADDR*)&clientAddr, &addrLen);
			if (recvLen <= 0) // 오류가 아니라면 성공적으로 msg 받음
				return 0;

			cout << "Recv Data : " << recvBuffer << endl;
			cout << "Recv Data Len : " << recvLen << endl;

			// TCP
			// int32 resultCode = ::send(clientSocket, recvBuffer, recvLen, 0);
			// if (resultCode == SOCKET_ERROR)
			//  	return 0;

			this_thread::sleep_for(1s);
		// }
	}

	// --------------------------

	// 끝
	::closesocket(listenSocket);
	::WSACleanup();

}