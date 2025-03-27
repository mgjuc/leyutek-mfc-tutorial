/*********************************************************************************************************
* 模块名称: UART.h
* 摘    要: 串口定义
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
#pragma once

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "Global.h"

/*********************************************************************************************************
*                                                宏定义
*********************************************************************************************************/
//一般串口的表述是"COM1","COM2",但是openport接口输入的端口都是用0表示com1，1表示com2
#define  MAP2PORT(port)   (port+1)

/*********************************************************************************************************
*                                               类的定义
*********************************************************************************************************/
class CUART
{
public:
	typedef int (*CALLFUNC)(void* pMsg, DWORD dwLength, void* pThis);  //CALLFUNC-回调函数  

	enum EnumPortNum
	{
		COM1,
		COM2,
		COM3,
		COM4,
		COM5
	};

	enum EnumFlowControl     // 串口流控制
	{
		No_FlowControl,      // 无流控制
		Cts_Rts_FlowControl, // 请求发送/清除发送流控制
		Cts_Dtr_FlowControl,
		Dsr_Rts_FlowControl,
		Dsr_Dtr_FlowControl, // 数据终端就绪/数据设置就绪流控制
		Xon_Xoff_FlowControl // 软件流控制
	};

	enum EnumRecvMode
	{
		Call_Func_Mode, // 回调函数模式
		Event_Mode      // 事件驱动模式
	};

public:
	CUART(); // 构造函数
	virtual ~CUART(); // 析构函数
	static unsigned _stdcall RecvThread(void* pParam); // 接收任务发送线程

public:
	int OpenPort(int nPort, int nRecvMode = Call_Func_Mode); // 打开一个端口，并初始化，默认CallFuncMode,调用时可以省略
	int ConfigPort(int nbaud = 0,
	int nparity = 0,
	int ndatabits = 3,
	int nstopbits = 0,
	EnumFlowControl fc = No_FlowControl
	); // 配置端口
	void RegisterCallFunc(CALLFUNC CallFunc, void* pThis); // 注册函数(回调函数必须为static) 
	int  RecvTaskProc(); // 接收事件消息函数
	int  SendData(LPCVOID pData, DWORD dwLen); // 发送数据
	int  RecvData(LPVOID  pData, DWORD dwLen); // 接收数据
	int  WaitComm();     // 等待串口事件
	void ClosePort();    // 关闭端口
	BOOL IsOpen() const; // 判断端口是否打开
	HANDLE mCommHandle;  // 端口句柄

private:
	OVERLAPPED mLSend;           // 端口发送OVERLAPPED结构
	OVERLAPPED mLRecv;           // 端口接收OVERLAPPED结构
	CALLFUNC   mRecvMsgCallFunc; // 接收回调函数指针
	void*      mRegisterPoint;   // 注册模块指针
	int        mRecvMode;        // 接收模式 
	HANDLE     mRecvMsgEvent;    // 接收消息通知事件句柄（回调模式用）
	HANDLE     mRecvThread;      // 接收线程句柄（回调模式用）
	BOOL       mRunThread;       // 接收任务运行标志
};
