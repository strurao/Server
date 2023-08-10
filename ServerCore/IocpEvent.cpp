#include "pch.h"
#include "IocpEvent.h"

/*--------------
	IocpEvent
	표를 발급받아서 우리가 어떤 이벤트를 실행할지 정해준다
---------------*/

IocpEvent::IocpEvent(EventType type) : type(type)
{
	Init();
}

void IocpEvent::Init()
{
	hEvent = 0;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
}
