#include <iostream>
using namespace std;

//基类
class CalcTime
{
	private:
		int mHour;  //小时值
		int mMin;   //分钟值
		
	public:
		int mSec;   //秒值
		
		static const int TIME_VAL_HOUR = 0x01;
		static const int TIME_VAL_MIN  = 0x02;
		
		//外部接口，输出转换的时间值
		int GetTimeVal(int type,int tick) 
		{
            int timeVal = 0;
			
			//tick对3600取模赋值给mHour
            mHour = tick / 3600;        
				
			//tick对3600取余后再对60取模赋值给mMin
            mMin  = (tick % 3600) / 60;
			
            switch(type)
            {
                case TIME_VAL_HOUR:
                    timeVal = mHour;
                    break;
                case TIME_VAL_MIN:
                    timeVal = mMin;
                    break;
                default:
                    break;
            }

            return timeVal;
        }
};

//类CalcAllTime通过公有继承方式继承基类CalcTime
class CalcAllTime: public CalcTime
{
	public:
		//外部接口，输出转换的秒值
		int GetSecVal(int tick)
		{
			int timeSec = 0;

            //tick对3600取余后再对60取余赋值给mSec
            mSec  = (tick % 3600) % 60; 
			timeSec = mSec;
	
			return timeSec;
		}
		
		int CalcFlg(int tick) 
		{
            int validFlag = 0;  //判断tick是否符合条件的标志位
			
            if(tick >= 0 && tick <= 86399) 
			{
                validFlag = 1;  //符合则返回1，然后转换时间     			
            }

            return validFlag;
        }
		
		//获取转换的时间并打印显示
		void DispTime(int tick) 
		{
			int hour; //小时值
			int min;  //分钟值
			int sec;  //秒值  
			
			//当tick的值在0~86399之间时，获取转换的时间值
			if(CalcFlg(tick) == 1) 
			{           
				hour = GetTimeVal(TIME_VAL_HOUR,tick);
				min  = GetTimeVal(TIME_VAL_MIN,tick);
				sec  = GetSecVal(tick);
			
				//打印转换之后的时间结果
				cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
			}
		}
};

int main()
{
	CalcAllTime ct;
	
	int tick = 0;   //0~86399    
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	ct.DispTime(tick);
	
	return 0;
}