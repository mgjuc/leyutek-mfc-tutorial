#include <iostream>
using namespace std;

//��������Ϊ�βΣ�����ʱʵ�ο�ֱ��Ϊ��������
void CalcSec(int& s_tick, int& s_sec)
{
	s_sec  = (s_tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��sec
}

int main()
{
	int tick = 0;   //0~86399    

    int hour; //Сʱֵ
    int min;  //����ֵ
    int sec;  //��ֵ
	
	//�������� 
    int& s_hour = hour; //Сʱֵ����
    int& s_min  = min;  //����ֵ����
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	s_hour = tick / 3600;        //tick��3600ȡģ��ֵ��hour
    s_min  = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��min
	CalcSec(tick, sec);          //ֱ�ӽ�������Ϊʵ�μ���sec��ֵ
	
	//��ӡת��֮���ʱ����
	cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
	
	return 0;
}