#include <iostream>
#include <conio.h>
#include "MyThreadPool.h"
#include <queue>
class A :public Itask
{
public:
	A(int a , int b)
	{
		this->a = a;
		this->b = b;
	}

	~A()
	{

	}
	void RunTask()
	{
		cout<<a<<"+"<<b<<"="<<a+b<<endl;
	}

private:
	int a ;
	int b;
};


int main()
{
	CMyThreadPool pool;
	pool.CreateThreadNum(10,1);
	for (int i = 0; i < 10000; i++)
	{
		Itask * task = new A(i,i+1);
		pool.AddTask(task);
	}
	system("pause");
	return 0;
}

// 19:29:37:952
// 19:29:49:813


//19:32:28:179
// 