#include <iostream>
using namespace std;

int main()
{
	int tick = 0;   //0~86399    

	int* p = new int[3]();  //动态分配3个int类型内存，同时内存空间全赋初值为0
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	if(NULL != p)  //对指针p判空
	{
		*(p + 2) =  tick / 3600;       //tick对3600取模赋值给*(p + 2)，即小时值
		*(p + 1) = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给*(p + 1)，即分钟值
		*(p + 0) = (tick % 3600) % 60; //tick对3600取余后再对60取余赋值给*(p + 0)，即秒值
		
		//打印转换之后的时间结果
		cout << "Current time : " << *(p + 2) << "-" << *(p + 1) << "-" << *(p + 0) << endl;
	}
	
	delete p; //释放内存空间，指示系统随时可回收内存，指针指向地址不变
	p = NULL; //指针指向0地址，即置空
	
	return 0;
}