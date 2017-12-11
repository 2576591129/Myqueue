#include <iostream>
#include <Windows.h>
using namespace std;

#define ARR_LEN 9999

template <typename T>
class Myqueue
{
public:
	Myqueue(int len )
	{
		memset( arr,0,ARR_LEN);
		arr_push = 0;
		arr_pop = 0;
		arr_size = 0;
		mysemaphore_push = NULL;
		InitializeCriticalSection(&cs_push);
		InitializeCriticalSection(&cs_pop);
		mysemaphore_push =  CreateSemaphore(0,0,1,0);
		this->len = len;
	}

	~Myqueue()
	{
		CloseHandle(mysemaphore_push);
		mysemaphore_push =NULL;
		CloseHandle(mysemaphore_push);
		mysemaphore_push =NULL;
		DeleteCriticalSection(&cs_push);
		DeleteCriticalSection(&cs_pop);

		for (int i = 0; i < len; i++)
		{
			delete arr[i];
			arr[i] = NULL;
		}

	}

	void push(T task )
	{
		if(IsFull())
			WaitForSingleObject(mysemaphore_push,INFINITE);
		EnterCriticalSection(&cs_push);
		if(IsFull())
			WaitForSingleObject(mysemaphore_push,INFINITE);
		arr[arr_push] =task;
		arr_push =  (arr_push+1)%ARR_LEN;
		arr_size +=1;
		LeaveCriticalSection(&cs_push);
	}

	T pop()
	{
		if(IsEmpty())return NULL;
		EnterCriticalSection(&cs_pop);
		if(IsEmpty())return NULL;
		T task = arr[arr_pop];
		arr[arr_pop] = NULL;
		arr_pop = (arr_pop+1)%ARR_LEN;
		arr_size-=1;
		if (arr_size + 1 == ARR_LEN)
			ReleaseSemaphore(mysemaphore_push,1,0);
		LeaveCriticalSection(&cs_pop);
		return task;
	}

	int size(){return arr_size;}

	bool IsEmpty()
	{ 
		if(arr_size == 0)return true;
		return false;
	}
	bool IsFull()
	{
		if (arr_size == ARR_LEN) return true;
		return false;
	}

	void Init(int len)
	{
		
	}


private:
	 int len ; 

	T  arr[ARR_LEN];
	int arr_push;
	int arr_pop;
	int arr_size;
	HANDLE mysemaphore_push;
	CRITICAL_SECTION cs_push;
	CRITICAL_SECTION cs_pop;
};