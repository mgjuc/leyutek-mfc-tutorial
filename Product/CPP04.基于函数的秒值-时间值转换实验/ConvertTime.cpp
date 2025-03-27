#include <iostream>
using namespace std;

int CalcHour(int tick) 
{
	int hour;
    hour = tick / 3600;       //tick对3600取模赋值给hour
    return(hour);
}

int CalcMin(int tick) 
{
    int min;
    min = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给min
    return(min);
}

int CalcSec(int tick) 
{
    int sec;
    sec = (tick % 3600) % 60; //tick对3600取余后再对60取余赋值给sec
    return(sec);
}
	
int main()
{
	int tick = 0;   //0~86399    

    int hour; //小时值
    int min;  //分钟值
    int sec;  //秒值  
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
    hour = CalcHour(tick);  //计算小时值
    min  = CalcMin(tick);   //计算分钟值
    sec  = CalcSec(tick);   //计算秒值
	
	//打印转换之后的时间结果
	cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
	
	return 0;
}