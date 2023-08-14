#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send
};

/*--------------
	IocpEvent
	티켓
---------------*/

struct IocpEvent : public OVERLAPPED
{
	IocpEvent(EventType type);

	void		Init();

	EventType	type;
	IocpObjectRef owner = nullptr; // 나의 주인님이 누구인가
	SessionRef session = nullptr; // Accept Only

	// TEMP
	vector<BYTE> buffer;
};