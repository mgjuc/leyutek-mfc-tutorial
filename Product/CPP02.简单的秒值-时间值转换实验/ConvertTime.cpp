#include <iostream>
using namespace std;

int main()
{
	int tick = 0;   //0~86399    

    int hour; //Сʱֵ
    int min;  //����ֵ
    int sec;  //��ֵ
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	hour = tick / 3600;        //tick��3600ȡģ��ֵ��hour
    min  = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��min
    sec  = (tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡ�ำֵ��sec
	
	//��ӡת��֮���ʱ����
	cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
	
	return 0;
}