#include <iostream>
using namespace std;

int main()
{
	int tick = 0;   //0~86399    

	int* p = new int[3]();  //��̬����3��int�����ڴ棬ͬʱ�ڴ�ռ�ȫ����ֵΪ0
	
	cout << "Please input a tick between 0~86399" << endl;
	cin  >> tick; 
	
	if(NULL != p)  //��ָ��p�п�
	{
		*(p + 2) =  tick / 3600;       //tick��3600ȡģ��ֵ��*(p + 2)����Сʱֵ
		*(p + 1) = (tick % 3600) / 60; //tick��3600ȡ����ٶ�60ȡģ��ֵ��*(p + 1)��������ֵ
		*(p + 0) = (tick % 3600) % 60; //tick��3600ȡ����ٶ�60ȡ�ำֵ��*(p + 0)������ֵ
		
		//��ӡת��֮���ʱ����
		cout << "Current time : " << *(p + 2) << "-" << *(p + 1) << "-" << *(p + 0) << endl;
	}
	
	delete p; //�ͷ��ڴ�ռ䣬ָʾϵͳ��ʱ�ɻ����ڴ棬ָ��ָ���ַ����
	p = NULL; //ָ��ָ��0��ַ�����ÿ�
	
	return 0;
}