#include <iostream>
using namespace std;

class CalcTime
{
	public:
		int mHour; //Сʱֵ
		int mMin;  //����ֵ
		int mSec;  //��ֵ
		
		void calcTimeVal(bool hourFlg, int tick)
		{
			if(hourFlg == 1 && tick >= 0 && tick <= 86399)
			{
				mHour = tick / 3600;        //tick��3600ȡģ��ֵ��mHour               
				
				//��ӡת��֮���ʱ����
				cout << "Current hourFlg : " << mHour << endl;
			}
		}
		
		void calcTimeVal(bool hourFlg, bool minFlg, int tick)
		{
			if(hourFlg == 1 && minFlg == 1 && tick >= 0 && tick <= 86399)
			{
				mHour = tick / 3600;        //tick��3600ȡģ��ֵ��mHour
				mMin  = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��mMin                
				
				//��ӡת��֮���ʱ����
				cout << "Current hourVal-minVal : " << mHour << "-" << mMin << endl;
			}
		}
		
		void calcTimeVal(bool hourFlg, bool minFlg, bool secFlg, int tick)
		{
			if(hourFlg == 1 && minFlg == 1 && secFlg == 1 && tick >= 0 && tick <= 86399)
			{
				mHour = tick / 3600;        //tick��3600ȡģ��ֵ��mHour
				mMin  = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��mMin
				mSec  = (tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡ�ำֵ��mSec                 
				
				//��ӡת��֮���ʱ����
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
