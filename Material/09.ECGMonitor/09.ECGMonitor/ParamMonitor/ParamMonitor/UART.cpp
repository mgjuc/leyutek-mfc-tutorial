/*********************************************************************************************************
* 模块名称: UART.cpp
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

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "pch.h"
#include "UART.h"
#include <process.h>
#include <iostream>

using namespace std;

/*********************************************************************************************************
* 函数名称: CUART
* 函数功能:	构造函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
CUART::CUART()
	: mCommHandle(INVALID_HANDLE_VALUE),
	mRecvMsgCallFunc(NULL),
	mRegisterPoint(NULL),
	mRecvMode(Call_Func_Mode),
	mRecvMsgEvent(NULL),
	mRecvThread(NULL),
	mRunThread(FALSE)
{
	ZeroMemory(&mLSend, sizeof(OVERLAPPED));
	ZeroMemory(&mLRecv, sizeof(OVERLAPPED));
}

/*********************************************************************************************************
* 函数名称: CUART
* 函数功能:	析构函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
CUART::~CUART()
{
	ClosePort(); // 关闭串口

	if (mRecvMsgEvent != NULL)
	{
		CloseHandle(mRecvMsgEvent); // 关闭接收信息通知事件线程句柄
		mRecvMsgEvent = NULL;
	}

	mRunThread = FALSE; // 接收任务运行标志为FALSE

	if (mRecvThread != NULL)
	{
		WaitForSingleObject(mRecvThread, INFINITE); // 等待接收线程函数退出
		CloseHandle(mRecvThread);                   // 关闭接收线程句柄
		mRecvThread = NULL;
	}

	mRecvMsgCallFunc = NULL; // 接收回调函数指针为NULL
	mRegisterPoint = NULL;   // 注册模块指针为NULL
}

/*********************************************************************************************************
* 函数名称: OpenPort
* 函数功能:	打开一个端口，并初始化
* 输入参数:	iPort-端口号，iRecvMode-接收模式
* 输出参数:	void
* 返 回 值: 成功返回DK_SUCCESS, 失败返回ERR_DK_FAILURE
* 创建日期: 2021年01月22日
* 注    意: 创建了一个接收线程，但是并不马上执行；而是挂起到mRecvMsgEvent事件上；
*           必须等调用ConfigPort后才能执行；
*********************************************************************************************************/
int CUART::OpenPort(int iPort, int iRecvMode)
{
	CString sPort;

	if (iPort < 10)
	{
		sPort.Format(_T("COM%d"), iPort);
	}
	else // 扫描串口号超过10的串口
	{
		sPort.Format(_T("\\\\.\\COM%d"), iPort);
	}

	//创建串口句柄，重叠方式
	mCommHandle = CreateFile(sPort,    // 创建串口
		GENERIC_WRITE | GENERIC_READ,  // 使用读/写方式
		0,                             // 不允许共享的设备
		NULL,                          // 安全属性设置
		OPEN_EXISTING,                 // 创建方式，只打开已有串口
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // 使用重叠方式
		NULL                           // 读操作时，指向模板文件句柄
	);

	if (mCommHandle == INVALID_HANDLE_VALUE) // 串口句柄无效的句柄值
	{
		return ERR_DK_FAILURE;
	}

	// 为重叠写创建事件对象，手工设置，初始化为无信号的
	ZeroMemory(&mLSend, sizeof(OVERLAPPED)); // 将指定的内存清零，一般在使用结构前清零
	mLSend.hEvent = CreateEvent(NULL,        // 该句柄不能被继承
		TRUE,                                // 人工重设
		FALSE,                               // 事件初始化状态为无信号状态
		NULL                                 // 指向事件名
	);
	if (mLSend.hEvent == INVALID_HANDLE_VALUE) // mLSend的hEvent为无效的句柄值
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// 为重叠读创建事件对象，手工设置，初始化为无信号的
	ZeroMemory(&mLRecv, sizeof(OVERLAPPED)); // 将指定的内存清零，一般在使用结构前清零
	mLRecv.hEvent = CreateEvent(NULL,        // 该句柄不能被继承
		TRUE,                                // 人工重设
		FALSE,                               // 事件初始化状态为无信号状态
		NULL                                 // 指向事件名
	);
	if (mLRecv.hEvent == INVALID_HANDLE_VALUE) // mLRecv的hEvent为无效的句柄值
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// 设置事件能用
	// SetCommMask-指定串口允许响应的事件为EV_RXCHAR,即接收到一个字符，并放入接收缓冲区
	if (!SetCommMask(mCommHandle, EV_RXCHAR))
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// 设置串口收发缓冲区
	if (!SetupComm(mCommHandle, MAXRECVBUF, MAXSENDBUF)) // 初始化串口的通信参数，0-失败，非零-成功
	{
		// 关闭串口
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// 清空串口缓冲区
	if (!PurgeComm(mCommHandle, PURGE_RXABORT | PURGE_TXABORT |
		PURGE_RXCLEAR | PURGE_TXCLEAR))
	{
		// 关闭串口
		ClosePort();
		return ERR_DK_FAILURE;
	}

	COMMTIMEOUTS commtimeouts;

	// 把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作
	commtimeouts.ReadIntervalTimeout = MAXDWORD; // 读间隔超时设置为最大
	commtimeouts.ReadTotalTimeoutMultiplier = 0; // 读时间系数为0
	commtimeouts.ReadTotalTimeoutConstant = 0;   // 读时间常量为0

	// 设置写超时以指定WriteComm成员函数中的GetOverlappedResult函数的等待时间
	commtimeouts.WriteTotalTimeoutMultiplier = 50; // 写时间系数为50
	commtimeouts.WriteTotalTimeoutConstant = 200;  // 写时间常数为200
	SetCommTimeouts(mCommHandle, &commtimeouts);   // 设置串口设备读写时的超时参数

	// 设置接收模式
	mRecvMode = iRecvMode;

	switch (mRecvMode)
	{
	case Call_Func_Mode:

		mRecvMsgEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // 创建接收消息时间句柄,自动重设
		if (mRecvMsgEvent == INVALID_HANDLE_VALUE)
		{
			ClosePort();
			return ERR_DK_FAILURE;
		}

		mRunThread = TRUE; // 接收任务运行标志为TRUE

		mRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvThread, this, 0, NULL); //创建接收线程，返回接收线程句柄
		if (mRecvThread == NULL)
		{
			ClosePort();
			return ERR_DK_FAILURE;
		}
		break;

	case Event_Mode:
		break;
	default:
		break;
	}

	return DK_SUCCESS;
}

/*********************************************************************************************************
* 函数名称: ConfigPort
* 函数功能:	配置端口，并启动接收线程
* 输入参数:	都是根据在Combo框中选择的顺序编号，编号从0开始
			baud - 波特率， parity - 奇偶校验，dataBits - 端口数据位
*           stopbits - 端口停止位数，fc - 端口控制流程
* 输出参数:	void
* 返 回 值: 成功返回DK_SUCCESS
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int CUART::ConfigPort(int nbaud, int nparity, int ndatabits,
	int nstopbits, EnumFlowControl fc)
{
	// 获得端口先前的状态
	DCB dcb; // 定义一个dcb结构存放串口通信参数
	dcb.DCBlength = sizeof(DCB); // DCB结构的大小
	if (!GetCommState(mCommHandle, &dcb)) // 打开串口后，获取串口的初始化配置
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// 通过修改DCB结构，来配置串口的各项参数
	// 设置二进制模式
	dcb.fBinary = TRUE;

	// 设置波特率
	switch (nbaud)
	{
	case 0: // 波特率4800
		dcb.BaudRate = 4800;
		break;
	case 1: // 波特率9600
		dcb.BaudRate = 9600;
		break;
	case 2: // 波特率14400
		dcb.BaudRate = 14400;
		break;
	case 3: // 波特率19200
		dcb.BaudRate = 19200;
		break;
	case 4: // 波特率38400
		dcb.BaudRate = 38400;
		break;
	case 5: // 波特率57600
		dcb.BaudRate = 57600;
		break;
	case 6: // 波特率115200
		dcb.BaudRate = 115200;
		break;
	default:
		break;
	}

	// 设置允许奇偶校验
	dcb.fParity = TRUE;

	// 设置奇偶校验
	switch (nparity)
	{
	case 0: // 无校验
		dcb.Parity = NOPARITY;
		break;
	case 1: // 奇数校验
		dcb.Parity = ODDPARITY;
		break;
	case 2: // 偶校验
		dcb.Parity = EVENPARITY;
		break;
	case 3: // 标志校验
		dcb.Parity = MARKPARITY;
		break;
	case 4: // 空格校验
		dcb.Parity = SPACEPARITY;
		break;
	default:
		break;
	}

	// 设置端口当前使用的数据位
	// dcb.ByteSize = byDataBits;

	switch (ndatabits)
	{
	case 0:
		dcb.ByteSize = 5;
		break;
	case 1:
		dcb.ByteSize = 6;
		break;
	case 2:
		dcb.ByteSize = 7;
		break;
	case 3:
		dcb.ByteSize = 8;
		break;
	default:
		break;
	}

	// 设置端口当前使用的停止数据位
	switch (nstopbits)
	{
	case 0: // 1位
		dcb.StopBits = ONESTOPBIT;
		break;
	case 1: // 1.5位
		dcb.StopBits = ONE5STOPBITS;
		break;
	case 2: // 2位
		dcb.StopBits = TWOSTOPBITS;
		break;
	default:
		break;
	}

	// 设定对DTR信号线是否敏感，如果为TRUE,那么DSR为OFF，接收的任何字节将被忽略
	dcb.fDsrSensitivity = FALSE;
	switch (fc)
	{
	case No_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Cts_Rts_FlowControl:
	{
		dcb.fOutxCtsFlow = TRUE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Cts_Dtr_FlowControl:
	{
		dcb.fOutxCtsFlow = TRUE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Dsr_Rts_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = TRUE;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Dsr_Dtr_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = TRUE;
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Xon_Xoff_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fOutX = TRUE;
		dcb.fInX = TRUE;
		dcb.XonChar = 0x11;
		dcb.XoffChar = 0x13;
		dcb.XoffLim = 100;
		dcb.XonLim = 100;
		break;
	}
	default:
		break;
	}

	// 重新设置当前端口状态
	if (!SetCommState(mCommHandle, &dcb))
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// 启动接收事件
	if (Call_Func_Mode == mRecvMode)
	{
		SetEvent(mRecvMsgEvent); // 将Event的状态设置为有信号状态
	}
	return DK_SUCCESS;
}

/*********************************************************************************************************
* 函数名称: RegisterCallFunc
* 函数功能:	队列重传子模块注册函数，注册好用户用于处理接收数据的线程函数
* 输入参数:	CallFunc - 注册函数指针，pThis - 注册对象指针
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CUART::RegisterCallFunc(CALLFUNC CallFunc, void* pThis)
{
	if (NULL == CallFunc || NULL == pThis)
	{
		return;
	}

	mRecvMsgCallFunc = CallFunc; // 接收回调函数指针为形参CallFunc
	mRegisterPoint = pThis;      // 注册模板函数中位置为形参pThis
}

/*********************************************************************************************************
* 函数名称: RecvThread
* 函数功能:	接收任务发送的线程调用执行的函数地址，名称就表示地址
* 输入参数:	pParam - 当前对象指针,
* 输出参数:	void
* 返 回 值: DK_SUCCESS
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
unsigned _stdcall CUART::RecvThread(void* pParam)
{
	CUART* pMyComm = NULL;

	if (NULL != pParam)
	{
		pMyComm = (CUART*)pParam;
		(void)pMyComm->RecvTaskProc();
	}
	return DK_SUCCESS;
}

/*********************************************************************************************************
* 函数名称: RecvTaskProc
* 函数功能:	接收事件消息函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: 成功返回DK_SUCCESS, 失败返回ERR_DK_FAILURE
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int CUART::RecvTaskProc()
{
	COMSTAT ComStat; // 通讯设备参数结构体
	DWORD dwErrorFlags = 0;
	int nError;
	HANDLE hMyComm = NULL;

	// 获取后台读命令的操作结果
	WaitForSingleObject(mRecvMsgEvent, INFINITE); // 使得线程自愿进入等待状态，直到变为已通知状态
	CloseHandle(mRecvMsgEvent);
	mRecvMsgEvent = NULL;

	while (mRunThread) // 接收任务运行标志为TRUE
	{
		if (mCommHandle == INVALID_HANDLE_VALUE)
		{
			break;
		}

		ClearCommError(mCommHandle, &dwErrorFlags, &ComStat); // 清除错误标志

		if (ComStat.cbInQue)
		{
			char szRecvBuf[MAXRECVBUF + 1] = { 0 };

			int nLen = 0;
			nLen = RecvData((void*)szRecvBuf, MAXRECVBUF); // 串口接收数据，放在szRecvBuf中

			if (nLen > 0)
			{
				mRecvMsgCallFunc(szRecvBuf, nLen, mRegisterPoint); // 回调函数
			}
			continue;
		}

		nError = WaitComm();
		if (ERR_DK_FAILURE == nError)
		{
			break;
		}
	}
	return ERR_DK_FAILURE;
}

/*********************************************************************************************************
* 函数名称: SendData
* 函数功能:	发送数据
* 输入参数:	pData - 发送的数据指针，dwLen - 发送数据长度
* 输出参数:	void
* 返 回 值: 成功返回发送成功的字节数, 失败返回ERR_DK_FAILURE
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int CUART::SendData(LPCVOID pData, DWORD dwLen)
{
	// 判断入口参数
	if (pData == NULL)
	{
		return ERR_DK_FAILURE;
	}

	BOOL bSuccess;
	DWORD dwErrorFlags;
	COMSTAT Comstat;
	DWORD dwSendCount = 0;
	DWORD dwError;

	ClearCommError(mCommHandle, &dwErrorFlags, &Comstat);

	// 发送数据
	bSuccess = WriteFile(mCommHandle, pData, dwLen, &dwSendCount, &mLSend);
	/*WriteFile函数：
	mCommHandle : 写入文件的句柄
	pData   : 写入文件的数据缓冲区指针
	dwLen   : 写入的字节数
	dwSendCount : 已发送的字节数
	mLSend: 重叠方式使用的结构体
	*/

	if (!bSuccess)
	{
		// 获得IO错误码
		dwError = GetLastError();
		if (ERROR_IO_PENDING == dwError) // 表示数据尚未发送完毕
		{
			// 一直等待发送成功
			bSuccess = GetOverlappedResult(mCommHandle, &mLSend, &dwSendCount, TRUE);
			if (!bSuccess)
			{
				return ERR_DK_FAILURE;
			}
		}
		else
		{
			return ERR_DK_FAILURE;
		}
	}
	return dwSendCount;
}

/*********************************************************************************************************
* 函数名称: RecvData
* 函数功能:	接收数据
* 输入参数:	pData - 接收的数据指针，dwLen - 接收数据长度
* 输出参数:	void
* 返 回 值: 成功返回接收的字节数, 失败返回ERR_DK_FAILURE
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int CUART::RecvData(LPVOID  pData, DWORD dwLen)
{
	// 判断入口参数
	if (pData == NULL)
	{
		return ERR_DK_FAILURE;
	}

	BOOL bSuccess = FALSE;
	DWORD dwErrorFlags;
	COMSTAT Comstat;
	DWORD dwRecvCount = 0;
	DWORD dwError;

	ClearCommError(mCommHandle, &dwErrorFlags, &Comstat);
	dwRecvCount = min(dwLen, Comstat.cbInQue); // 取实际接收的数据长度与接收缓冲长度之间的最小值

	// 读事件发生
	bSuccess = ReadFile(mCommHandle, pData, dwLen, &dwRecvCount, &mLRecv);

	if (!bSuccess)
	{
		// 获得IO错误码
		dwError = GetLastError();
		if (ERROR_IO_PENDING == dwError) // 表示数据尚未发送完毕
		{
			// 一直等待接收成功
			bSuccess = GetOverlappedResult(mCommHandle, &mLRecv, &dwRecvCount, TRUE);
			if (!bSuccess)
			{
				return ERR_DK_FAILURE;
			}
		}
		else
		{
			return ERR_DK_FAILURE;
		}
	}
	return dwRecvCount;
}

/*********************************************************************************************************
* 函数名称: WaitComm
* 函数功能:	等待串口事件
* 输入参数:	void
* 输出参数:	void
* 返 回 值: 成功返回DK_SUCCESS, 失败返回ERR_DK_FAILURE
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
int CUART::WaitComm()
{
	OVERLAPPED os;
	DWORD dwMark = 0;
	DWORD dwTrans = 0;

	os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (os.hEvent == NULL)
	{
		return ERR_DK_FAILURE;
	}

	if (!WaitCommEvent(mCommHandle, &dwMark, &os)) // 重叠操作
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// 无限等待重叠操作结果
			GetOverlappedResult(mCommHandle, &os, &dwTrans, TRUE);
		}
		else
		{
			CloseHandle(os.hEvent);
			return ERR_DK_FAILURE;
		}
	}

	CloseHandle(os.hEvent);
	return DK_SUCCESS;
}

/*********************************************************************************************************
* 函数名称: ClosePort
* 函数功能:	关闭端口,同时,串口关闭将引发接收线程结束
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void CUART::ClosePort()
{
	BOOL bOpen = FALSE;

	try
	{
		bOpen = IsOpen();
		mRunThread = FALSE;
		if (TRUE == bOpen)
		{
			// 关闭当前打开的端口句柄
			(void)CloseHandle(mCommHandle);
			mCommHandle = INVALID_HANDLE_VALUE;

			// 关闭当前事件句柄
			if (mLSend.hEvent != NULL)
			{
				CloseHandle(mLSend.hEvent);
			}

			if (mLRecv.hEvent != NULL)
			{
				CloseHandle(mLRecv.hEvent);
			}

			ZeroMemory(&mLSend, sizeof(OVERLAPPED));
			ZeroMemory(&mLRecv, sizeof(OVERLAPPED));
		}
	}
	catch (...)
	{
		cout << "close port failed!" << endl;
	}
}

/*********************************************************************************************************
* 函数名称: IsOpen
* 函数功能:	检查是否已经打开一个端口
* 输入参数:	void
* 输出参数:	void
* 返 回 值: 成功返回TRUE, 失败返回FALSE
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
BOOL CUART::IsOpen() const
{
	return (INVALID_HANDLE_VALUE != mCommHandle);
}
