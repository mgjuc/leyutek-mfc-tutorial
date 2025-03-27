#include <iostream>
using namespace std;

//�ӿڣ��ɼ̳У����ɴ�������
class Interface
{
	public:
		virtual void DispTime() = 0;
		virtual void CalcHour(int tick) = 0;
		virtual void CalcMin(int tick) = 0;
		virtual void CalcSec(int tick) = 0;
};

//��CalcTimeͨ�����м̳з�ʽ���̳�Interface��
class CalcTime: public Interface
{
	public:
		int mHour; //Сʱֵ
		int mMin;  //����ֵ
		int mSec;  //��ֵ
		
		void CalcHour(int tick) 
		{
            mHour = tick / 3600;       //tick��3600ȡģ��ֵ��mHour
        }
		
		void CalcMin(int tick) 
		{
            mMin = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��mMin
        }
		
		void CalcSec(int tick) 
		{
            mSec = (tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡ�ำֵ��mSec
        }
		
		void DispTime() 
		{
            //��ӡת��֮���ʱ����
			cout << "Current time : " << mHour << "-" << mMin << "-" << mSec << endl;   
        }
};

int main()
{
	CalcTime ct;
	
	int tick = 0;   //0~86399  
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	//ת��ʱ�䲢��ʾ
	ct.CalcHour(tick);
	ct.CalcMin(tick);
	ct.CalcSec(tick);
	
	ct.DispTime();
	
	return 0;
}
