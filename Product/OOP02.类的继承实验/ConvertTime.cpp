#include <iostream>
using namespace std;

//����
class CalcTime
{
	private:
		int mHour;  //Сʱֵ
		int mMin;   //����ֵ
		
	public:
		int mSec;   //��ֵ
		
		static const int TIME_VAL_HOUR = 0x01;
		static const int TIME_VAL_MIN  = 0x02;
		
		//�ⲿ�ӿڣ����ת����ʱ��ֵ
		int GetTimeVal(int type,int tick) 
		{
            int timeVal = 0;
			
			//tick��3600ȡģ��ֵ��mHour
            mHour = tick / 3600;        
				
			//tick��3600ȡ����ٶ�60ȡģ��ֵ��mMin
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

//��CalcAllTimeͨ�����м̳з�ʽ�̳л���CalcTime
class CalcAllTime: public CalcTime
{
	public:
		//�ⲿ�ӿڣ����ת������ֵ
		int GetSecVal(int tick)
		{
			int timeSec = 0;

            //tick��3600ȡ����ٶ�60ȡ�ำֵ��mSec
            mSec  = (tick % 3600) % 60; 
			timeSec = mSec;
	
			return timeSec;
		}
		
		int CalcFlg(int tick) 
		{
            int validFlag = 0;  //�ж�tick�Ƿ���������ı�־λ
			
            if(tick >= 0 && tick <= 86399) 
			{
                validFlag = 1;  //�����򷵻�1��Ȼ��ת��ʱ��     			
            }

            return validFlag;
        }
		
		//��ȡת����ʱ�䲢��ӡ��ʾ
		void DispTime(int tick) 
		{
			int hour; //Сʱֵ
			int min;  //����ֵ
			int sec;  //��ֵ  
			
			//��tick��ֵ��0~86399֮��ʱ����ȡת����ʱ��ֵ
			if(CalcFlg(tick) == 1) 
			{           
				hour = GetTimeVal(TIME_VAL_HOUR,tick);
				min  = GetTimeVal(TIME_VAL_MIN,tick);
				sec  = GetSecVal(tick);
			
				//��ӡת��֮���ʱ����
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