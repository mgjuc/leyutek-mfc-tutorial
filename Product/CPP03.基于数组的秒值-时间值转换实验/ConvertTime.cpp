#include <iostream>
using namespace std;

int main()
{
	int tick = 0;   //0~86399    

	int arrTimeVal[3];
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
    arrTimeVal[2] =  tick / 3600;       //tick对3600取模赋值给arrTimeVal[2]，即小时值
    arrTimeVal[1] = (tick % 3600) / 60; //tick对3600取余后再对60取模赋值给arrTimeVal[1]，即分钟值
    arrTimeVal[0] = (tick % 3600) % 60; //tick对3600取余后再对60取余赋值给arrTimeVal[0]，即秒值
	
	//打印转换之后的时间结果
	cout << "Current time : " << arrTimeVal[2] << "-" << arrTimeVal[1] << "-" << arrTimeVal[0] << endl;
	
	return 0;
}