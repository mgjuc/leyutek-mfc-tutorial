#include <iostream>
using namespace std;

enum EnumTimeVal
{
	TIME_VAL_HOUR,
	TIME_VAL_MIN,
	TIME_VAL_SEC,
	TIME_VAL_MAX
};

int CalcTimeVal(int tick, EnumTimeVal type)
{
	int TimeVal = 0;
	
	switch(type)
	{
		case TIME_VAL_HOUR:
			TimeVal = tick / 3600;
			break;
		case TIME_VAL_MIN:
			TimeVal = (tick % 3600) / 60;
			break;
		case TIME_VAL_SEC:
			TimeVal = (tick % 3600) % 60;
		default:
			break;
	}
	
	return TimeVal;
}

int main()
{
	int tick = 0;   //0~86399    

    int hour; //Сʱֵ
    int min;  //����ֵ
    int sec;  //��ֵ 
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick;
	
	hour = CalcTimeVal(tick, TIME_VAL_HOUR);
	min  = CalcTimeVal(tick, TIME_VAL_MIN);
	sec  = CalcTimeVal(tick, TIME_VAL_SEC);

	//��ӡת��֮���ʱ����
	cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
	
	return 0;
}

