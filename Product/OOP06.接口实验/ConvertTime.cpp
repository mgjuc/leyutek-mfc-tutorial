#include <iostream>
using namespace std;

//接口，可继承，不可创建对象
class Interface
{
	public:
		virtual void DispTime() = 0;
		virtual void CalcHour(int tick) = 0;
		virtual void CalcMin(int tick) = 0;
		virtual void CalcSec(int tick) = 0;
};

//类CalcTime通过公有继承方式来继承Interface类
class CalcTime: public Interface
{
	public:
		int mHour; //小时值
		int mMin;  //分钟值
		int mSec;  //秒值
		
		void CalcHour(int tick) 
		{
            mHour = tick / 3600;       //tick对3600取模赋值给mHour
        }
		
		void CalcMin(int tick) 
		{
            mMin = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给mMin
        }
		
		void CalcSec(int tick) 
		{
            mSec = (tick % 3600) % 60; //tick对3600取余后再对60取余赋值给mSec
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
