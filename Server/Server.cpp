#include <iostream>
using namespace std;
#include <thread> // c++ 표준 라이브러리
#include <vector>
#include "windows.h"
#include <atomic> // 원자적! 공유자원과 연관이 있다. 그러나 교통정리를 하느라 훨씬 느리다
#include <Windows.h>


/*
영역    주의   사유                                 예

Code    X     딱히 수정하지 않음
Stack   X     스레드마다 자신만의 고유스택영역을 사용   지역변수
Heap    O     공유하므로 복잡한 문제가 생길 수 있다
Data    O     공유하므로 복잡한 문제가 생길 수 있다    전역/static변수 (코어끼리의 공유자원)
*/


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


/*
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
*/
