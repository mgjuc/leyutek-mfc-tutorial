/*********************************************************************************************************
* 模块名称: system.cpp
* 使用说明: 系统大部分全局变量定义；初始化函数定义；自检结果显示功能的实现；波速调节功能的实现；
* 摘    要: 系统初始化；自检结果显示功能；波速调节功能；
* 当前版本: 1.0.0
* 作    者: Leyutek(COPYRIGHT 2018 - 2021 Leyutek. All rights reserved.)
* 完成日期: 2021年01月22日
* 内    容:
* 注    意: none
**********************************************************************************************************
* 取代版本:
* 作    者:
* 完成日期:
* 修改内容:
* 修改文件:
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "pch.h"
#include "System.h"
#include "UART.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              全局变量
*********************************************************************************************************/
// 串口配置
StructUARTConfig  gUARTConfig;

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitConfig
* 函数功能:	初始化系统参数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void InitConfig(void)
{
	gUARTConfig.parity = 0; // 默认选择第0项：无校验
	gUARTConfig.baudRate = 6; // 默认选择第6项：115200
	gUARTConfig.port = 0; // 默认选择第1项端口
	gUARTConfig.dataBits = 3; // 默认选择第3项：8位数据位
	gUARTConfig.stopBits = 0; // 默认选择第0项：1位停止位

}

/*********************************************************************************************************
* 函数名称: SplitString
* 函数功能: 按照给定的符号进行字符串的分割
* 输入参数: str-要拆分的字符串，split-给定的符号，strArray-拆分后的字符串数组
* 输出参数:
* 返 回 值: 返回字符串分割后的个数
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int SplitString(const CString str, char split, CStringArray& strArray)
{
	strArray.RemoveAll(); // 清零
	CString strTemp = str;
	int index = 0;
	while(1)
	{
		index = strTemp.Find(split); // 找到指定字符的位置
		if(index >= 0)
		{
			strArray.Add(strTemp.Left(index)); // CString.left从左边开始获取前iIndex个字符
			strTemp = strTemp.Right(strTemp.GetLength() - index - 1);
		}
		else
		{
			break;
		}
	}
	strArray.Add(strTemp); // 增加最后一个字符串

	return strArray.GetSize();
}