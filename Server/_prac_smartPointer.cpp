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

/* 절대 수동으로 refCount를 관리하지 않도록, 모든 상황에서 대해서 refCount를 자동으로 해준다*/
template<typename T>
class TSharedPtr
{
public:
	TSharedPtr() {}
	TSharedPtr(T* ptr) { Set(ptr); }

	// 복사... 어딘가에서 복사가 일어난다면 AddRef() 가 호출된다
	TSharedPtr(const TSharedPtr& other) { Set(other._ptr); }
	// 이동
	TSharedPtr(TSharedPtr&& other) { _ptr = other._ptr; other._ptr = nullptr; }

	// 상속 관계 복사
	template<typename U>
	TSharedPtr(const TSharedPtr<U>& rhs) { set(static_cast<T*>(rhs._ptr)); }
	~TSharedPtr() { Release(); }

public:
	// 복사 연산자
	TSharedPtr& operator=(const TSharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release(); // 내가 들고 있던 걸 날리기
			Set(rhs._ptr);
		}
		return *this;
	}

	// 이동 연산자
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