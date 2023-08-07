#include <iostream>
using namespace std;
#include <thread> 
#include <vector>
#include "windows.h"
#include <atomic> 
#include <Windows.h>
#include <queue>
#include <mutex>

/* 이벤트와 조건 변수 */ 

mutex m;
queue<int> q;

HANDLE hEvent; // 기내화장실에서 대기할 때, 자리가 비면 알려달라고 승무원님께 부탁드린다 -> 이벤트
// 하나의 정수에 불과하나, 정수가 특정한 커널오브젝트를 지칭한다. 운영체제가 관리하는 오브젝트가 커널 오브젝트.

condition_variable cv; // 조건변수. 커널 오브젝트가 아니라 유저레벨 오브젝트. 이벤트까지 안가도 된다.
/* 
CV 사용법
1) Lock 잡기
2) 공유 변수 값 수정
3) Lock 풀고
4) CV 통해서 통지
*/



// 일감을 찍어내는 쪽
void Producer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		q.push(100);

		// ::SetEvent(hEvent); // 신호 바뀜. 여기선 꺼져있다가(빨간불) 이제 켜짐(파란불)
		cv.notify_one(); // 대기중인 스레드가 있다면 딱 1개를 깨운다 (통지)

		this_thread::sleep_for(100ms);
	}
}

// 이렇게 역할분담을 하자~
void Consumer()
{
	while (true)
	{
		// ::WaitForSingleObject(hEvent, INFINITE); // 파란불이 될 때까지 대기하는 중
		// ::ResetEvent(hEvent);

		unique_lock<mutex> lock(m); // 왜 unique를 쓰냐? 중간에 풀어줄 수 있어서.
		cv.wait(lock, []() {return q.empty() == false; }); // predicate(조건)이 묶여있다.
		/*
		1) Lock을 잡으려고 시도 (이미 잡혔으면 스킵)
		2) 조건 확인
			만족O -> 바로 빠져나와 이어서 코드 진행
			만족X -> Lock 풀고 대기로 전환 (잠들어버린다)
		*/
		
		
		if (q.empty() == false) // 이벤트의 단점
		{
			int data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}




/*
int x = 0;
int y = 0;
int r1 = 0;
int r2 = 0;

bool ready = false; 

void Thread_1()
{
	while (ready == false){}
	y = 1; // Store Y
	r1 = x; // Load X
}

void Thread_2 ()
{
	while (ready == false){}
	x = 1;
	r2 = y;
}
*/

int main()
{
	/*
	커널 오브젝트 EVENT
	- Usage Count
	- Signal (파란불) / Non-Signal (빨간불)
	*/

	// Auto / Manual < bool
	// 삼국지의 봉화대처럼 껐다 켰다 할 수 있다
	hEvent = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*초기상태*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(hEvent);
	/*
	int count = 0;
	while (true)
	{
		ready = false;
		count++;

		x = y = r1 = r2 = 0;

		thread t1(Thread_1);
		thread t2(Thread_2);

		ready = true;

		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
			break;
	}
	cout << count << endl;
	*/
}
