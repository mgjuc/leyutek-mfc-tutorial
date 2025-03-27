#include <iostream>
using namespace std;

class CalcTime
{
	private:
		int mHour;  //小时值
		int mMin;   //分钟值
		int mSec;   //秒值
		
	public:
		static const int TIME_VAL_HOUR = 0x01;
		static const int TIME_VAL_MIN  = 0x02;
		static const int TIME_VAL_SEC  = 0x03;
		
		//当tick的值在0~86399之间时，获取转换的时间值
		int CalcTimeVal(int tick) 
		{
            int validFlag = 0;  //判断tick是否符合条件的标志位
            
            if(tick >= 0 && tick <= 86399) 
			{
                validFlag = 1;  //符合则返回1，然后转换时间
                
				//tick对3600取模赋值给mHour
                mHour = tick / 3600; 
				
				//tick对3600取余后再对60取模赋值给mMin
                mMin  = (tick % 3600) / 60;  
				
				//tick对3600取余后再对60取余赋值给mSec
                mSec  = (tick % 3600) % 60;  
            }

            return validFlag;
        }
		
		//外部接口，输出转换的时间值
		int GetTimeVal(int type) 
		{
            int timeVal = 0;

            switch(type)
            {
                case TIME_VAL_HOUR:
                    timeVal = mHour;
                    break;
                case TIME_VAL_MIN:
                    timeVal = mMin;
                    break;
                case TIME_VAL_SEC:
                    timeVal = mSec;
                    break;
                default:
                    break;
            }

            return timeVal;
        }
};

class ConvertTime 
{
	private:
	    CalcTime ct;
		
    public:
		//获取转换的时间并打印显示
		void DispTime(int tick) 
		{
			int hour; //小时值
			int min;  //分钟值
			int sec;  //秒值  
			
			//当tick的值在0~86399之间时，获取转换的时间值
			if(ct.CalcTimeVal(tick) == 1) 
			{           
				hour = ct.GetTimeVal(ct.TIME_VAL_HOUR);
				min  = ct.GetTimeVal(ct.TIME_VAL_MIN);
				sec  = ct.GetTimeVal(ct.TIME_VAL_SEC);
			
				//打印转换之后的时间结果
				cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
			}
		}
};

int main()
{
	ConvertTime convert;
	
	int tick = 0;   //0~86399    
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	convert.DispTime(tick);
	
	return 0;
}