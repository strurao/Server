#include "pch.h"
#include <iostream>

// 클라
// 1) 새로운 소켓 생성  // 문의용 핸드폰
// 2) 서버에 연결 요청
// 3) 서버와 통신
int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0
	// return : descriptor
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr; // 내가 접속할 상대방의 IPv4 주소
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); << deprecated
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 루프백 주소 127.0.0.1
	serverAddr.sin_port = ::htons(7777); // 80 : HTTP

	// 바인딩 대신 커넥트
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	// ---------------------------
	// 연결 성공! 이제부터 데이터 송수신 가능!
	cout << "Connected To Server!" << endl;

	while (true)
	{
		// TODO

		char sendBuffer[100] = "Hello! I am Client";
		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
			return 0;

		char recvBuffer[100];
		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0)
			return 0;

		cout << "Echo Data : " << recvBuffer << endl;

		this_thread::sleep_for(1s);
	}

	// --------------------------

	::closesocket(clientSocket);
	::WSACleanup();
}