#include <iostream>
using namespace std;

int main()
{
	int tick = 0;   //0~86399    

	int arrTimeVal[3];
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
    arrTimeVal[2] =  tick / 3600;       //tick��3600ȡģ��ֵ��arrTimeVal[2]����Сʱֵ
    arrTimeVal[1] = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��arrTimeVal[1]��������ֵ
    arrTimeVal[0] = (tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡ�ำֵ��arrTimeVal[0]������ֵ
	
	//��ӡת��֮���ʱ����
	cout << "Current time : " << arrTimeVal[2] << "-" << arrTimeVal[1] << "-" << arrTimeVal[0] << endl;
	
	return 0;
}