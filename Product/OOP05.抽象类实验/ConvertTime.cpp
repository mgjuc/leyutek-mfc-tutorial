#include <iostream>
using namespace std;

//�����࣬���ܴ�������ֻ�����ڼ̳�
class Time
{
	public:
		int mHour; //Сʱֵ
		int mMin;  //����ֵ
		int mSec;  //��ֵ
		
		//����ָʾ��������ǰ������Ϊ���鷽�����ԴﵽʹTime���Ϊ������
		virtual void DispTime() = 0;
};

//CalcTime��ͨ�����м̳з�ʽ���̳�Time��
class CalcTime: public Time
{
	public:
		void CalcHour(int tick) 
		{
            mHour = tick / 3600;       //tick��3600ȡģ��ֵ��hour
        }
		
		void CalcMin(int tick) 
		{
            mMin = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��min
        }
		
		void CalcSec(int tick) 
		{
            mSec = (tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡ�ำֵ��sec
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
