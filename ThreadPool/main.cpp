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
	{
		CMyThreadPool pool;
	}


	system("pause");
	return 0;
}

// 19:29:37:952
// 19:29:49:813


//19:32:28:179
// 