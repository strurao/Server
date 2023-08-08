#include "pch.h"
#include "CoreTLS.h"
 
/*
Code
Stack
Heap
Data
TLS    전역변수이기는 하나, 동일한 스레드 내에서 전역으로 사용가능 (예: ThreadID)

thread_local 키워드: 
스레드마다 별도의 인스턴스를 갖는 변수를 선언할 때 사용됩니다. 
이 키워드를 사용하면 각 스레드가 독립적인 값을 가진 변수를 사용할 수 있게 되어, 
데이터 경합이 없고 스레드 간에 데이터를 공유하지 않아도 됩니다.
*/

thread_local uint32 LThreadId = 0;