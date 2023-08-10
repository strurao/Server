#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	Recv,
	Send
};

/*--------------
	IocpEvent
	Ƽ��
---------------*/

struct IocpEvent : public OVERLAPPED
{
	IocpEvent(EventType type);

	void		Init();

	EventType	type;
	Session* session = nullptr; // Accept Only
};