#include "MyThreadPool.h"


CMyThreadPool::CMyThreadPool(void):myqueue(10)
{
	flag_thread_quit = true;
	num_create_thread =0 ;
	num_run_thread = 0;
	num_max_thread = 0 ;
	InitializeCriticalSection(&cs);
	mysemaphore = NULL;

}


CMyThreadPool::~CMyThreadPool(void)
{
	DestoryThread();
}

bool CMyThreadPool::CreateThreadNum(int max_num,int  min_num)
{
	if (max_num < min_num || min_num <=0)return false;
	mysemaphore =  CreateSemaphore(0,0,max_num,0);
	for (int i = 0; i < min_num; i++)
	{
		HANDLE handle = (HANDLE) _beginthreadex(0,0,ThreadProc,this,0,0);
		if(handle)
			list_thread_handle.push_back(handle);
	}
	num_create_thread = min_num;
	num_max_thread = max_num;
	return true;
}

void CMyThreadPool::DestoryThread()	
{
	flag_thread_quit = false;
	if(mysemaphore != NULL)
	{
		CloseHandle(mysemaphore);
		mysemaphore =NULL;
	}
	if (!myqueue.IsEmpty())
	{
		Itask * task = myqueue.pop();
		delete (task);
	}

	if(list_thread_handle.empty()) return;
	EnterCriticalSection(&cs);
	if(list_thread_handle.empty()) 
	{ 
		LeaveCriticalSection(&cs);
		return;
	}
	auto iter = list_thread_handle.begin();
	while (iter != list_thread_handle.end())
	{
		if (WaitForSingleObject(*iter,100) == WAIT_TIMEOUT)
			TerminateThread(*iter,-999);
		CloseHandle(*iter);
		*iter = NULL;
		iter = list_thread_handle.erase(iter);
	}
	LeaveCriticalSection(&cs);

}

bool CMyThreadPool::AddTask(Itask * task)
{
	if(task == NULL)return false;
	myqueue.push(task);

	if (num_run_thread < num_create_thread )
	{
		ReleaseSemaphore(mysemaphore,1,0);
	}
	else if (num_create_thread < num_max_thread)
	{
		HANDLE handle = (HANDLE) _beginthreadex(0,0,ThreadProc,this,0,0);
		if(handle)
			list_thread_handle.push_back(handle);

		ReleaseSemaphore(mysemaphore,1,0);
		num_create_thread ++;
	}
	return true;
}

unsigned  __stdcall CMyThreadPool::ThreadProc( void * lpama)
{
	CMyThreadPool * pthis = (CMyThreadPool*) lpama;
	Itask *	task = NULL  ;
	while (pthis->flag_thread_quit)
	{
		if (WaitForSingleObject(pthis->mysemaphore,100) == WAIT_TIMEOUT)continue;
		pthis->num_run_thread ++ ;
		while (!pthis->myqueue.IsEmpty())
		{		
			task = pthis->myqueue.pop();
			task->RunTask();	
			delete task;
			task = NULL;
		}
		pthis->num_run_thread --;
	}
	return 0;
}


