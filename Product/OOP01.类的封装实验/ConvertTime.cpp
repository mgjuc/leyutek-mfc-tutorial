#include <iostream>
using namespace std;

class CalcTime
{
	private:
		int mHour;  //Сʱֵ
		int mMin;   //����ֵ
		int mSec;   //��ֵ
		
	public:
		static const int TIME_VAL_HOUR = 0x01;
		static const int TIME_VAL_MIN  = 0x02;
		static const int TIME_VAL_SEC  = 0x03;
		
		//��tick��ֵ��0~86399֮��ʱ����ȡת����ʱ��ֵ
		int CalcTimeVal(int tick) 
		{
            int validFlag = 0;  //�ж�tick�Ƿ���������ı�־λ
            
            if(tick >= 0 && tick <= 86399) 
			{
                validFlag = 1;  //�����򷵻�1��Ȼ��ת��ʱ��
                
				//tick��3600ȡģ��ֵ��mHour
                mHour = tick / 3600; 
				
				//tick��3600ȡ����ٶ�60ȡģ��ֵ��mMin
                mMin  = (tick % 3600) / 60;  
				
				//tick��3600ȡ����ٶ�60ȡ�ำֵ��mSec
                mSec  = (tick % 3600) % 60;  
            }

            return validFlag;
        }
		
		//�ⲿ�ӿڣ����ת����ʱ��ֵ
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
		//��ȡת����ʱ�䲢��ӡ��ʾ
		void DispTime(int tick) 
		{
			int hour; //Сʱֵ
			int min;  //����ֵ
			int sec;  //��ֵ  
			
			//��tick��ֵ��0~86399֮��ʱ����ȡת����ʱ��ֵ
			if(ct.CalcTimeVal(tick) == 1) 
			{           
				hour = ct.GetTimeVal(ct.TIME_VAL_HOUR);
				min  = ct.GetTimeVal(ct.TIME_VAL_MIN);
				sec  = ct.GetTimeVal(ct.TIME_VAL_SEC);
			
				//��ӡת��֮���ʱ����
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