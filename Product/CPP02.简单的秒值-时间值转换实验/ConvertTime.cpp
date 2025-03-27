#include <iostream>
using namespace std;

int main()
{
	int tick = 0;   //0~86399    

    int hour; //小时值
    int min;  //分钟值
    int sec;  //秒值
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	hour = tick / 3600;        //tick对3600取模赋值给hour
    min  = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给min
    sec  = (tick % 3600) % 60; //tick对3600取余后再对60取余赋值给sec
	
	//打印转换之后的时间结果
	cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
	
	return 0;
}