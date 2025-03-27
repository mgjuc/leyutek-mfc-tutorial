#include <iostream>
#include <exception>
using namespace std;

class CalcTime
{
	public:
		int mHour; //小时值
		int mMin;  //分钟值
		int mSec;  //秒值
		
		void CalcTimeVal(int tick)
		{
			if(tick < 0 || tick > 86399) 
			{
				throw "Tick value is not valid!!"; //抛出异常信息         
			} 
			else 
			{
				mHour = tick / 3600;        //tick对3600取模赋值给mHour
				mMin  = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给mMin
				mSec  = (tick % 3600) % 60; //tick对3600取余后再对60取余赋值给mSec                 
				
				//打印转换之后的时间结果
				cout << "Current time : " << mHour << "-" << mMin << "-" << mSec << endl;
			}
		}
};

int main()
{
	CalcTime ct;
	
	int tick = 0;   //0~86399    
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick;
	
	//保护代码
	try 
	{
		ct.CalcTimeVal(tick);
    }
	//捕获异常信息并输出提示
    catch(const char* e) 
	{
        cout << e << endl;
    }
	
	return 0;
}