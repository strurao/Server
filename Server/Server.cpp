#include <iostream>
using namespace std;
#include <thread> // c++ 표준 라이브러리
#include <vector>
#include "windows.h"
#include <atomic> // 원자적! 공유자원과 연관이 있다. 그러나 교통정리를 하느라 훨씬 느리다
#include <Windows.h>
#include <mutex> // Lock

/*
영역    주의   사유                                 예

Code    X     딱히 수정하지 않음
Stack   X     스레드마다 자신만의 고유스택영역을 사용   지역변수
Heap    O     공유하므로 복잡한 문제가 생길 수 있다
Data    O     공유하므로 복잡한 문제가 생길 수 있다    전역/static변수 (코어끼리의 공유자원)
*/


// Mutual Exclusive (상호배타적: 한 번에 하나의 스레드만 실행할 수 있다)
// 만약 3개의 스레드가 접근하려면, 누가 먼저 접근할지 알 수 없다. 가장 빠른 녀석이 자물쇠를 풀고 (임계영역) 화장실을 사용하는 격이다.
// 1,2,3, 중에서 1번이 먼저 갔다가 또다시 1번이 또 들어갈 수도 있다. 아닐 수도 있고. 이것이 경합이다.
// RW Lock같은 특이 케이스를 제외하고 기본적으로 Lock은 한번에 하나이다. 그것이 동기화이다
// ex) airplane toilet
mutex m;

vector<int> v;


// RAII (Resource Acquisition Is Initialization)
// 일종의 Wrapper Class 이다
// std::lock_guard
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m) : _mutex(m)
	{
		_mutex.lock();
	}

	~LockGuard()
	{
		_mutex.unlock();
	}

private:
	T& _mutex;
};


void Push()
{
	for (int i = 0; i < 10000; i++)
	{
		// m.lock();
		// v.push_back(i);
		// m.unlock(); // 자물쇠처럼 잠그고 풀기. 이걸 주석으로 하면 나머지 애들은 영영 대기이다.
		//LockGuard<mutex> lockGuard(m);
		std::lock_guard<mutex> lockGuard(m); // 이렇게 모두 다 lock을 거는 것은 정답이 아니다

		//std::unique_lock<mutex> uniqueLock(m, std::defer_lock); // lock_guard 와 거의 동일하나, 개발자 수동 잠금
		//uniqueLock.lock();

		v.push_back(i);

		if (i == 5000)
		{
			break;
		}	
	}
}


void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (SHORT) x, (SHORT)y };
	::SetConsoleCursorPosition(output, pos);
}

void SetCursorOnOff(bool visible)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	::GetConsoleCursorInfo(output, &cursorInfo);
	cursorInfo.bVisible = visible;
	::SetConsoleCursorInfo(output, &cursorInfo);
}

void HelloThread(int i)
{
	while(true)
	{
		cout << "Hello Thread " << i << endl;
	}
}



int main()
{
	vector<thread> threads;

	// 직원 배치하기
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(thread(HelloThread, i));
	}

	std::thread t(HelloThread, 100); // 프로그램은 하나인데 영혼은 두 개

	cout << "Hello Main " << endl;

	for (thread& t : threads)
	{
		t.get_id(); // 임의의 id 로 구분해볼 수 있겠다
		int a = t.hardware_concurrency(); // 병렬처리 가능한 CPU 코어 개수. 내 PC는 16개이다
		t.join(); // 끝날 때까지 대기한다
	}
		
	// SetCursorOnOff(false);
}

