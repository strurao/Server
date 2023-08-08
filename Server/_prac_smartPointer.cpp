#include <iostream>
using namespace std;
#include <thread> 
#include <vector>
#include "windows.h"
#include <atomic> 
#include <Windows.h>
#include <queue>
#include <mutex>

class RefCountable
{
public:
	RefCountable() {}
	virtual ~RefCountable() {}

	int GetRefCount() { return _refCount; }
	int AddRef() { return ++_refCount; }
	int ReleaseRef()
	{
		int refCount = --_refCount;
		if (refCount == 0)
			delete this;

		return refCount;
	}

protected:
	atomic<int> _refCount = 1;
};

/* ���� �������� refCount�� �������� �ʵ���, ��� ��Ȳ���� ���ؼ� refCount�� �ڵ����� ���ش�*/
template<typename T>
class TSharedPtr
{
public:
	TSharedPtr() {}
	TSharedPtr(T* ptr) { Set(ptr); }

	// ����... ��򰡿��� ���簡 �Ͼ�ٸ� AddRef() �� ȣ��ȴ�
	TSharedPtr(const TSharedPtr& other) { Set(other._ptr); }
	// �̵�
	TSharedPtr(TSharedPtr&& other) { _ptr = other._ptr; other._ptr = nullptr; }

	// ��� ���� ����
	template<typename U>
	TSharedPtr(const TSharedPtr<U>& rhs) { set(static_cast<T*>(rhs._ptr)); }
	~TSharedPtr() { Release(); }

public:
	// ���� ������
	TSharedPtr& operator=(const TSharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release(); // ���� ��� �ִ� �� ������
			Set(rhs._ptr);
		}
		return *this;
	}

	// �̵� ������
	TSharedPtr& operator=(TSharedPtr&& rhs)
	{
		Release();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}

	bool     operator==(const TSharedPtr& rhs) const { return _ptr == rhs._ptr; }
	bool     operator==(T* ptr)const { return _ptr == ptr; }
	bool     operator!=(const TSharedPtr& rhs) const { return _ptr != rhs._ptr; }
	bool     operator!=(T* ptr)const { return _ptr != ptr; }
	bool     operator<(const TSharedPtr& rhs) const { return _ptr < rhs._ptr; }
	T*       operator*() { return _ptr; }
	const T* operator*() const { return _ptr; }
	T*       opertator T* () const { return _ptr; }
	T*       operator->() { return _ptr; }
	const T* operator->() const { return _ptr; }

	bool IsNull() { return _ptr == nullptr; }

private:
	void Set(T* ptr)
	{
		_ptr = ptr;
		if (ptr)
			ptr->AddRef();
	}
	void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}
	
private:
	T* _ptr = nullptr;
};


class Knight :public RefCountable
{
public:

};

using KnightRef = TSharedPtr<Knight>;

void Test(KnightRef knight)
{

}

//int main()
//{
//	KnightRef knightSP(new Knight());
//	Test(knightSP);
//
//	/*Knight* knight = new Knight();
//	knight->ReleaseRef();*/
//}