#include <iostream>
using namespace std;

int CalcHour(int tick) 
{
	int hour;
    hour = tick / 3600;       //tick��3600ȡģ��ֵ��hour
    return(hour);
}

int CalcMin(int tick) 
{
    int min;
    min = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��min
    return(min);
}

int CalcSec(int tick) 
{
    int sec;
    sec = (tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡ�ำֵ��sec
    return(sec);
}
	
int main()
{
	int tick = 0;   //0~86399    

    int hour; //Сʱֵ
    int min;  //����ֵ
    int sec;  //��ֵ  
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
    hour = CalcHour(tick);  //����Сʱֵ
    min  = CalcMin(tick);   //�������ֵ
    sec  = CalcSec(tick);   //������ֵ
	
	//��ӡת��֮���ʱ����
	cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
	
	return 0;
}