#include <iostream>
using namespace std;

//把引用作为形参，调用时实参可直接为变量本身
void CalcSec(int& s_tick, int& s_sec)
{
	s_sec  = (s_tick % 3600) % 60; //tick对3600取余后再对60取模赋值给sec
}

int main()
{
	int tick = 0;   //0~86399    

    int hour; //小时值
    int min;  //分钟值
    int sec;  //秒值
	
	//定义引用 
    int& s_hour = hour; //小时值引用
    int& s_min  = min;  //分钟值引用
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	s_hour = tick / 3600;        //tick对3600取模赋值给hour
    s_min  = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给min
	CalcSec(tick, sec);          //直接将变量作为实参计算sec的值
	
	//打印转换之后的时间结果
	cout << "Current time : " << hour << "-" << min << "-" << sec << endl;
	
	return 0;
}