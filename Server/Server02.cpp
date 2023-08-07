#include <iostream>
using namespace std;
#include <thread> 
#include <vector>
#include "windows.h"
#include <atomic> 
#include <Windows.h>

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

/*
int main()
{
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
}
*/