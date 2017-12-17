#pragma once
#include <queue>
#include <list>
#include <process.h>

#include "Myqueue.h"
using namespace std;


class Itask
{
public:
	Itask(){}
	virtual ~Itask(){}
	virtual void RunTask()
	{

	}
};


class CMyThreadPool
{
public:
	CMyThreadPool(void);
	~CMyThreadPool(void);

public:
	bool CreateThreadNum(int max_num,int  min_num);
	void DestoryThread();

	static unsigned  __stdcall ThreadProc( void * lpama);

	bool AddTask(Itask * task);


private:


	Myqueue<Itask *>  myqueue;

	list<HANDLE> list_thread_handle;
	HANDLE mysemaphore;
	bool flag_thread_quit;
	CRITICAL_SECTION cs;
	int num_create_thread;
	int num_run_thread;
	int num_max_thread;
};

