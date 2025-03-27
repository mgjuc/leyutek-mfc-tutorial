#include <iostream>
using namespace std;

//基类
class CalcTime
{
	private:
		int mHour;  //小时值
		
	public:
		//这个方法加上virtual关键字来修饰它可被继承的子类重写，称为虚方法，实现多态
		virtual int GetTimeVal(int tick) 
		{    
			//tick对3600取模赋值给mHour
            mHour = tick / 3600; 
			
            return mHour;
        }
};

//类CalcMin通过公有继承方式继承基类CalcTime
class CalcMin: public CalcTime
{
	private:
		int mMin;   //分钟值
		
	public:	
		//外部接口，输出转换的时间值
		int GetTimeVal(int tick) 
		{
			//tick对3600取余后再对60取模赋值给mMin
            mMin  = (tick % 3600) / 60;
			
            return mMin;
        }
};

//类CalcSec通过公有继承方式继承基类CalcTime
class CalcSec: public CalcTime
{
	private:
		int mSec;   //秒值
		
	public:			
		//外部接口，输出转换的秒值
		int GetTimeVal(int tick)
		{
			//tick对3600取余后再对60取余赋值给mSec
            mSec  = (tick % 3600) % 60;		
			
			return mSec;
		}
};

class ConvertTime 
{
	private:
		CalcTime* ct;     //创建一个CalcTime类型的指针
		CalcTime ctHour;  //创建一个CalcTime类的对象
		CalcMin  ctMin;   //创建一个CalcMin类的对象
		CalcSec  ctSec;   //创建一个CalcSec类的对象
		
    public:		
		//获取转换的时间并打印显示
		void DispTime(int tick) 
		{
			int hour; //小时值
			int min;  //分钟值
			int sec;  //秒值  
			
			//当tick的值在0~86399之间时，获取转换的时间值
			if(tick >= 0 && tick <= 86399) 
			{
				ct   = &ctHour;
				hour = ct -> GetTimeVal(tick);
				
				ct   = &ctMin;
				min  = ct -> GetTimeVal(tick);
				
				ct   = &ctSec;
				sec  = ct -> GetTimeVal(tick);
			
				//打印转换之后的时间结果
				cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
			}
			else
			{
				cout << "Tick value is not valid!!" << endl;
			}
		}
};

int main()
{
	ConvertTime ct;
	
	int tick = 0;   //0~86399    
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	ct.DispTime(tick);
	
	return 0;
}