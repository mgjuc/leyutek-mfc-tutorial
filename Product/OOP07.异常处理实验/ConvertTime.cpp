#include <iostream>
#include <exception>
using namespace std;

class CalcTime
{
	public:
		int mHour; //Сʱֵ
		int mMin;  //����ֵ
		int mSec;  //��ֵ
		
		void CalcTimeVal(int tick)
		{
			if(tick < 0 || tick > 86399) 
			{
				throw "Tick value is not valid!!"; //�׳��쳣��Ϣ         
			} 
			else 
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
	
	//��������
	try 
	{
		ct.CalcTimeVal(tick);
    }
	//�����쳣��Ϣ�������ʾ
    catch(const char* e) 
	{
        cout << e << endl;
    }
	
	return 0;
}