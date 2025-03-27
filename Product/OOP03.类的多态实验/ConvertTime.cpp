#include <iostream>
using namespace std;

//����
class CalcTime
{
	private:
		int mHour;  //Сʱֵ
		
	public:
		//�����������virtual�ؼ������������ɱ��̳е�������д����Ϊ�鷽����ʵ�ֶ�̬
		virtual int GetTimeVal(int tick) 
		{    
			//tick��3600ȡģ��ֵ��mHour
            mHour = tick / 3600; 
			
            return mHour;
        }
};

//��CalcMinͨ�����м̳з�ʽ�̳л���CalcTime
class CalcMin: public CalcTime
{
	private:
		int mMin;   //����ֵ
		
	public:	
		//�ⲿ�ӿڣ����ת����ʱ��ֵ
		int GetTimeVal(int tick) 
		{
			//tick��3600ȡ����ٶ�60ȡģ��ֵ��mMin
            mMin  = (tick % 3600) / 60;
			
            return mMin;
        }
};

//��CalcSecͨ�����м̳з�ʽ�̳л���CalcTime
class CalcSec: public CalcTime
{
	private:
		int mSec;   //��ֵ
		
	public:			
		//�ⲿ�ӿڣ����ת������ֵ
		int GetTimeVal(int tick)
		{
			//tick��3600ȡ����ٶ�60ȡ�ำֵ��mSec
            mSec  = (tick % 3600) % 60;		
			
			return mSec;
		}
};

class ConvertTime 
{
	private:
		CalcTime* ct;     //����һ��CalcTime���͵�ָ��
		CalcTime ctHour;  //����һ��CalcTime��Ķ���
		CalcMin  ctMin;   //����һ��CalcMin��Ķ���
		CalcSec  ctSec;   //����һ��CalcSec��Ķ���
		
    public:		
		//��ȡת����ʱ�䲢��ӡ��ʾ
		void DispTime(int tick) 
		{
			int hour; //Сʱֵ
			int min;  //����ֵ
			int sec;  //��ֵ  
			
			//��tick��ֵ��0~86399֮��ʱ����ȡת����ʱ��ֵ
			if(tick >= 0 && tick <= 86399) 
			{
				ct   = &ctHour;
				hour = ct -> GetTimeVal(tick);
				
				ct   = &ctMin;
				min  = ct -> GetTimeVal(tick);
				
				ct   = &ctSec;
				sec  = ct -> GetTimeVal(tick);
			
				//��ӡת��֮���ʱ����
				cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
			}
			else
			{
				cout << "Tick value is not valid!!" << endl;
			}
		}
};

int main()
{
	ConvertTime ct;
	
	int tick = 0;   //0~86399    
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	ct.DispTime(tick);
	
	return 0;
}