#include <iostream>
using namespace std;

class CalcTime
{
	public:
		int mHour; //小时值
		int mMin;  //分钟值
		int mSec;  //秒值
		
		void calcTimeVal(bool hourFlg, int tick)
		{
			if(hourFlg == 1 && tick >= 0 && tick <= 86399)
			{
				mHour = tick / 3600;        //tick对3600取模赋值给mHour               
				
				//打印转换之后的时间结果
				cout << "Current hourFlg : " << mHour << endl;
			}
		}
		
		void calcTimeVal(bool hourFlg, bool minFlg, int tick)
		{
			if(hourFlg == 1 && minFlg == 1 && tick >= 0 && tick <= 86399)
			{
				mHour = tick / 3600;        //tick对3600取模赋值给mHour
				mMin  = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给mMin                
				
				//打印转换之后的时间结果
				cout << "Current hourVal-minVal : " << mHour << "-" << mMin << endl;
			}
		}
		
		void calcTimeVal(bool hourFlg, bool minFlg, bool secFlg, int tick)
		{
			if(hourFlg == 1 && minFlg == 1 && secFlg == 1 && tick >= 0 && tick <= 86399)
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
	
	ct.calcTimeVal(1,tick);
	ct.calcTimeVal(1,1,tick);
	ct.calcTimeVal(1,1,1,tick);
	
	return 0;
}
