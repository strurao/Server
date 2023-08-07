#include <iostream>
using namespace std;
#include <thread> 
#include <vector>
#include "windows.h"
#include <atomic> 
#include <Windows.h>
#include <queue>
#include <mutex>

/* �̺�Ʈ�� ���� ���� */ 

mutex m;
queue<int> q;

HANDLE hEvent; // �⳻ȭ��ǿ��� ����� ��, �ڸ��� ��� �˷��޶�� �¹����Բ� ��Ź�帰�� -> �̺�Ʈ
// �ϳ��� ������ �Ұ��ϳ�, ������ Ư���� Ŀ�ο�����Ʈ�� ��Ī�Ѵ�. �ü���� �����ϴ� ������Ʈ�� Ŀ�� ������Ʈ.

condition_variable cv; // ���Ǻ���. Ŀ�� ������Ʈ�� �ƴ϶� �������� ������Ʈ. �̺�Ʈ���� �Ȱ��� �ȴ�.
/* 
CV ����
1) Lock ���
2) ���� ���� �� ����
3) Lock Ǯ��
4) CV ���ؼ� ����
*/



// �ϰ��� ���� ��
void Producer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		q.push(100);

		// ::SetEvent(hEvent); // ��ȣ �ٲ�. ���⼱ �����ִٰ�(������) ���� ����(�Ķ���)
		cv.notify_one(); // ������� �����尡 �ִٸ� �� 1���� ����� (����)

		this_thread::sleep_for(100ms);
	}
}

// �̷��� ���Һд��� ����~
void Consumer()
{
	while (true)
	{
		// ::WaitForSingleObject(hEvent, INFINITE); // �Ķ����� �� ������ ����ϴ� ��
		// ::ResetEvent(hEvent);

		unique_lock<mutex> lock(m); // �� unique�� ����? �߰��� Ǯ���� �� �־.
		cv.wait(lock, []() {return q.empty() == false; }); // predicate(����)�� �����ִ�.
		/*
		1) Lock�� �������� �õ� (�̹� �������� ��ŵ)
		2) ���� Ȯ��
			����O -> �ٷ� �������� �̾ �ڵ� ����
			����X -> Lock Ǯ�� ���� ��ȯ (���������)
		*/
		
		
		if (q.empty() == false) // �̺�Ʈ�� ����
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
	Ŀ�� ������Ʈ EVENT
	- Usage Count
	- Signal (�Ķ���) / Non-Signal (������)
	*/

	// Auto / Manual < bool
	// �ﱹ���� ��ȭ��ó�� ���� �״� �� �� �ִ�
	hEvent = ::CreateEvent(NULL/*���ȼӼ�*/, FALSE/*bManualReset*/, FALSE/*�ʱ����*/, NULL);

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
