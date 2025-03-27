#include <iostream>
using namespace std;

//抽象类，不能创建对象，只能用于继承
class Time
{
	public:
		int mHour; //小时值
		int mMin;  //分钟值
		int mSec;  //秒值
		
		//用于指示编译器当前声明的为纯虚方法，以达到使Time类成为抽象类
		virtual void DispTime() = 0;
};

//CalcTime类通过公有继承方式来继承Time类
class CalcTime: public Time
{
	public:
		void CalcHour(int tick) 
		{
            mHour = tick / 3600;       //tick对3600取模赋值给hour
        }
		
		void CalcMin(int tick) 
		{
            mMin = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给min
        }
		
		void CalcSec(int tick) 
		{
            mSec = (tick % 3600) % 60; //tick对3600取余后再对60取余赋值给sec
        }
		
		void DispTime() 
		{
            //打印转换之后的时间结果
			cout << "Current time : " << mHour << "-" << mMin << "-" << mSec << endl;   
        }
};		

int main()
{
	CalcTime ct;
	
	int tick = 0;   //0~86399    
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	//转换时间并显示
	ct.CalcHour(tick);
	ct.CalcMin(tick);
	ct.CalcSec(tick);
	
	ct.DispTime();
	
	return 0;
}
