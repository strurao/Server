#include "pch.h"
#include <iostream>
#include <thread> // c++ ǥ�� ���̺귯��
#include <vector>
#include "windows.h"
#include <atomic> // ������! �����ڿ��� ������ �ִ�. �׷��� ���������� �ϴ��� �ξ� ������
#include <Windows.h>
#include <mutex> // Lock
#include <queue>
using namespace std;


/*
����    ����   ����                                 ��

Code    X     ���� �������� ����
Stack   X     �����帶�� �ڽŸ��� �������ÿ����� ���   ��������
Heap    O     �����ϹǷ� ������ ������ ���� �� �ִ�    �����迭
Data    O     �����ϹǷ� ������ ������ ���� �� �ִ�    ����/static���� (�ھ���� �����ڿ�)
*/


// Mutual Exclusive (��ȣ��Ÿ��: �� ���� �ϳ��� �����常 ������ �� �ִ�)
// ���� 3���� �����尡 �����Ϸ���, ���� ���� �������� �� �� ����. ���� ���� �༮�� �ڹ��踦 Ǯ�� (�Ӱ迵��) ȭ����� ����ϴ� ���̴�.
// 1,2,3, �߿��� 1���� ���� ���ٰ� �Ǵٽ� 1���� �� �� ���� �ִ�. �ƴ� ���� �ְ�. �̰��� �����̴�.
// RW Lock���� Ư�� ���̽��� �����ϰ� �⺻������ Lock�� �ѹ��� �ϳ��̴�. �װ��� ����ȭ�̴�
// Ex) Airplane's Toilet
mutex m;

vector<int> v;


// RAII (Resource Acquisition Is Initialization)
// ������ Wrapper Class �̴�
// std::lock_guard
/*
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
*/

// �츮���� ��
class Lock
{
public:
	void lock()
	{
		// �Ʒ��� �ڵ�� ��ȣ��Ÿ���� ��߳���. �����尡 ƴ���� ���� �� �ִ�
		/*
		while (_flag)
		{
			// Do Nothing
		}
		_flag = true;
		*/

		// �׷��� ����ϴ� CAS (Compare-And-Swap)
		bool expected = false;
		bool desired = true;

		// Spin Lock: spin�� ��� ������ ���� �����ʰ� ��ũ�� �Ѵٴ� �� (������Ż)
		// ����: 
		// ����: CPU cost �� ũ��. �⳻ȭ��� ��� ���ε� �ջ���� ���� �� �����ٸ�?
		while (_flag.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}

		// �̰��� �����ڵ�
		/*
		if (_flag == expected)
		{
			_flag = desired;
			return true;
		}
		else
		{
			expected = _flag;
			return false;
		}
		*/
	}

	void unlock()
	{
		_flag = false;
	}

private:
	atomic<bool> _flag = false;
};

Lock m2;

void Push()
{
	for (int i = 0; i < 10000; i++)
	{
		// m.lock();
		// v.push_back(i);
		// m.unlock(); // �ڹ���ó�� ��װ� Ǯ��. �̰� �ּ����� �ϸ� ������ �ֵ��� ���� ����̴�.
		//LockGuard<mutex> lockGuard(m);

		//std::lock_guard<mutex> lockGuard(m); // �̷��� ��� �� lock�� �Ŵ� ���� ������ �ƴϴ�

		//std::unique_lock<mutex> uniqueLock(m, std::defer_lock); // lock_guard �� ���� �����ϳ�, ������ ���� ���
		//uniqueLock.lock();

		std::lock_guard<Lock> lockGuard(m2);

		v.push_back(i);

		if (i == 5000)
		{
			break;
		}
	}
}

void HelloThread(int i)
{
	while (true)
	{
		cout << "Hello Thread " << i << endl;
	}
}


/*int main()
{
	//
	vector<thread> threads;

	// ���� ��ġ�ϱ�
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(thread(HelloThread, i));
	}

	std::thread t(HelloThread, 100); // ���α׷��� �ϳ��ε� ��ȥ�� �� ��

	cout << "Hello Main " << endl;

	for (thread& t : threads)
	{
		t.get_id(); // ������ id �� �����غ� �� �ְڴ�
		int a = t.hardware_concurrency(); // ����ó�� ������ CPU �ھ� ����. �� PC�� 16���̴�
		t.join(); // ���� ������ ����Ѵ�
	}
	//

	v.reserve(100000);

	thread t1(Push);
	thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}
*/





////////////////////////////////////////////////






/* �̺�Ʈ�� ���� ���� */ 

/mutex m;
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

// ctrl k /
//int main()
//{
//	/*
//	Ŀ�� ������Ʈ EVENT
//	- Usage Count
//	- Signal (�Ķ���) / Non-Signal (������)
//	*/
//
//	// Auto / Manual < bool
//	// �ﱹ���� ��ȭ��ó�� ���� �״� �� �� �ִ�
//
//	hEvent = ::CreateEvent(NULL/*���ȼӼ�*/, FALSE/*bManualReset*/, FALSE/*�ʱ����*/, NULL);
//
//	thread t1(Producer);
//	thread t2(Consumer);
//
//	t1.join();
//	t2.join();
//
//	::CloseHandle(hEvent);
//	///*
//	int count = 0;
//	while (true)
//	{
//		ready = false;
//		count++;
//
//		x = y = r1 = r2 = 0;
//
//		thread t1(Thread_1);
//		thread t2(Thread_2);
//
//		ready = true;
//
//		t1.join();
//		t2.join();
//
//		if (r1 == 0 && r2 == 0)
//			break;
//	}
//	cout << count << endl;
//	//*/
//}

