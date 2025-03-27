#include "pch.h"

#include  "Uart.h"
#include  <process.h>   //用到_beginthreadex函数
#include  <iostream>

using namespace std;

/*********************************************************************************************************
* 函数名称: CUart
* 函数功能:	构造函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
CUart::CUart()
    : m_hComm(INVALID_HANDLE_VALUE),
    m_RecvMsgCallFunc(NULL),
    m_pRegisterPoint(NULL),
    m_nRecvMode(Call_Func_Mode),
    m_hRecvMsgEvent(NULL),
    m_hRecvThread(NULL),
    m_bRunThread(FALSE)
{
    ZeroMemory(&m_olSend, sizeof(OVERLAPPED));
    ZeroMemory(&m_olRecv, sizeof(OVERLAPPED));

}

/*********************************************************************************************************
* 函数名称: CUart
* 函数功能:	析构函数
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
CUart::~CUart()
{
    ClosePort();   //关闭串口

    if (m_hRecvMsgEvent != NULL)
    {
        CloseHandle(m_hRecvMsgEvent);  //关闭接收信息通知事件线程句柄
        m_hRecvMsgEvent = NULL;
    }

    m_bRunThread = FALSE;  //接收任务运行标志为FALSE

    if (m_hRecvThread != NULL)
    {
        WaitForSingleObject(m_hRecvThread, INFINITE);  //等待接收线程函数退出
        CloseHandle(m_hRecvThread);                    //关闭接收线程句柄
        m_hRecvThread = NULL;
    }

    m_RecvMsgCallFunc = NULL;   //接收回调函数指针为NULL
    m_pRegisterPoint = NULL;   //注册模块指针为NULL
}

/*********************************************************************************************************
* 函数名称: OpenPort
* 函数功能:	打开一个端口，并初始化
* 输入参数:	iPort-端口号，iRecvMode-接收模式
* 输出参数:	void
* 返 回 值: 成功返回DK_SUCCESS, 失败返回ERR_DK_FAILURE
* 创建日期: 2017年08月29日
* 注    意: 创建了一个接收线程，但是并不马上执行；而是挂起到m_hRecvMsgEvent事件上；
*           必须等调用ConfigPort后才能执行；
*********************************************************************************************************/
int CUart::OpenPort(int iPort, int iRecvMode)
{
    CString sPort;

    if (iPort < 10)
    {
        sPort.Format(_T("COM%d"), iPort);
    }
    else  //扫描串口号超过10的串口
    {
        sPort.Format(_T("\\\\.\\COM%d"), iPort);
    }

    //创建串口句柄，重叠方式
    m_hComm = CreateFile(sPort,              //创建串口
        GENERIC_WRITE | GENERIC_READ,  //使用读/写方式
        0,                             //不允许共享的设备
        NULL,                          //安全属性设置
        OPEN_EXISTING,                 //创建方式，只打开已有串口
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,   //使用重叠方式
        NULL                           //读操作时，指向模板文件句柄
    );

    if (m_hComm == INVALID_HANDLE_VALUE)     //串口句柄无效的句柄值
    {
        return ERR_DK_FAILURE;
    }

    //为重叠写创建事件对象，手工设置，初始化为无信号的
    ZeroMemory(&m_olSend, sizeof(OVERLAPPED));            //将指定的内存清零，一般在使用结构前清零
    m_olSend.hEvent = CreateEvent(NULL,                   //该句柄不能被继承
        TRUE,                   //人工重设
        FALSE,                  //事件初始化状态为无信号状态
        NULL                    //指向事件名
    );
    if (m_olSend.hEvent == INVALID_HANDLE_VALUE)           //m_olSend的hEvent为无效的句柄值
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //为重叠读创建事件对象，手工设置，初始化为无信号的
    ZeroMemory(&m_olRecv, sizeof(OVERLAPPED));            //将指定的内存清零，一般在使用结构前清零
    m_olRecv.hEvent = CreateEvent(NULL,                   //该句柄不能被继承
        TRUE,                   //人工重设
        FALSE,                  //事件初始化状态为无信号状态
        NULL                    //指向事件名
    );
    if (m_olRecv.hEvent == INVALID_HANDLE_VALUE)           //m_olRecv的hEvent为无效的句柄值
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //设置事件能用
    //SetCommMask-指定串口允许响应的事件为EV_RXCHAR,即接收到一个字符，并放入接收缓冲区
    if (!SetCommMask(m_hComm, EV_RXCHAR))
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //设置串口收发缓冲区
    if (!SetupComm(m_hComm, MAXRECVBUF, MAXSENDBUF))   //初始化串口的通信参数，0-失败，非零-成功
    {
        //关闭串口
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //清空串口缓冲区
    if (!PurgeComm(m_hComm, PURGE_RXABORT | PURGE_TXABORT |
        PURGE_RXCLEAR | PURGE_TXCLEAR))
    {
        //关闭串口
        ClosePort();
        return ERR_DK_FAILURE;
    }

    COMMTIMEOUTS commtimeouts;

    //把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作
    commtimeouts.ReadIntervalTimeout = MAXDWORD;       //读间隔超时设置为最大
    commtimeouts.ReadTotalTimeoutMultiplier = 0;              //读时间系数为0
    commtimeouts.ReadTotalTimeoutConstant = 0;              //读时间常量为0

    //设置写超时以指定WriteComm成员函数中的GetOverlappedResult函数的等待时间
    commtimeouts.WriteTotalTimeoutMultiplier = 50;            //写时间系数为50
    commtimeouts.WriteTotalTimeoutConstant = 200;           //写时间常数为200
    SetCommTimeouts(m_hComm, &commtimeouts);                  //设置串口设备读写时的超时参数

    //设置接收模式
    m_nRecvMode = iRecvMode;

    switch (m_nRecvMode)
    {
    case Call_Func_Mode:

        m_hRecvMsgEvent = CreateEvent(NULL, FALSE, FALSE, NULL);    //创建接收消息时间句柄,自动重设
        if (m_hRecvMsgEvent == INVALID_HANDLE_VALUE)
        {
            ClosePort();
            return ERR_DK_FAILURE;
        }

        m_bRunThread = TRUE;   //接收任务运行标志为TRUE

        m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvThread, this, 0, NULL); //创建接收线程，返回接收线程句柄
        if (m_hRecvThread == NULL)
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
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
int CUart::ConfigPort(int nbaud, int nparity, int ndatabits,
    int nstopbits, EnumFlowControl fc)
{
    //获得端口先前的状态
    DCB dcb;          //定义一个dcb结构存放串口通信参数
    dcb.DCBlength = sizeof(DCB);      //DCB结构的大小
    if (!GetCommState(m_hComm, &dcb))  //打开串口后，获取串口的初始化配置
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //通过修改DCB结构，来配置串口的各项参数
    //设置二进制模式
    dcb.fBinary = TRUE;

    //设置波特率
    switch (nbaud)
    {
    case 0:        //波特率4800
        dcb.BaudRate = 4800;
        break;
    case 1:       //波特率9600
        dcb.BaudRate = 9600;
        break;
    case 2:       //波特率14400
        dcb.BaudRate = 14400;
        break;
    case 3:       //波特率19200
        dcb.BaudRate = 19200;
        break;
    case 4:       //波特率38400
        dcb.BaudRate = 38400;
        break;
    case 5:       //波特率57600
        dcb.BaudRate = 57600;
        break;
    case 6:       //波特率115200
        dcb.BaudRate = 115200;
        break;
    default:
        break;
    }

    //设置允许奇偶校验
    dcb.fParity = TRUE;

    //设置奇偶校验
    switch (nparity)
    {
    case 0:        //无校验
        dcb.Parity = NOPARITY;
        break;
    case 1:       //奇数校验
        dcb.Parity = ODDPARITY;
        break;
    case 2:       //偶校验
        dcb.Parity = EVENPARITY;
        break;
    case 3:       //标志校验
        dcb.Parity = MARKPARITY;
        break;
    case 4:       //空格校验
        dcb.Parity = SPACEPARITY;
        break;
    default:
        break;
    }

    //设置端口当前使用的数据位
    //dcb.ByteSize = byDataBits;

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

    //设置端口当前使用的停止数据位
    switch (nstopbits)
    {
    case 0:          //1位
        dcb.StopBits = ONESTOPBIT;
        break;
    case 1:          //1.5位
        dcb.StopBits = ONE5STOPBITS;
        break;
    case 2:          //2位
        dcb.StopBits = TWOSTOPBITS;
        break;
    default:
        break;
    }


    //设定对DTR信号线是否敏感，如果为TRUE,那么DSR为OFF，接收的任何字节将被忽略
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

    //重新设置当前端口状态
    if (!SetCommState(m_hComm, &dcb))
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //启动接收事件
    if (Call_Func_Mode == m_nRecvMode)
    {
        SetEvent(m_hRecvMsgEvent);   //将Event的状态设置为有信号状态
    }
    return DK_SUCCESS;
}

/*********************************************************************************************************
* 函数名称: RegisterCallFunc
* 函数功能:	队列重传子模块注册函数，注册好用户用于处理接收数据的线程函数
* 输入参数:	CallFunc - 注册函数指针，pThis - 注册对象指针
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
void CUart::RegisterCallFunc(CALLFUNC CallFunc, void* pThis)
{
    if (NULL == CallFunc || NULL == pThis)
    {
        return;
    }

    m_RecvMsgCallFunc = CallFunc;     //接收回调函数指针为形参CallFunc
    m_pRegisterPoint = pThis;        //注册模板函数中位置为形参pThis
}

/*********************************************************************************************************
* 函数名称: RecvThread
* 函数功能:	接收任务发送的线程调用执行的函数地址，名称就表示地址
* 输入参数:	pParam - 当前对象指针,
* 输出参数:	void
* 返 回 值: DK_SUCCESS
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
unsigned _stdcall CUart::RecvThread(void* pParam)
{
    CUart* pMyComm = NULL;

    if (NULL != pParam)
    {
        pMyComm = (CUart*)pParam;
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
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
int CUart::RecvTaskProc()
{
    COMSTAT ComStat;  //通讯设备参数结构体
    DWORD dwErrorFlags = 0;
    int nError;
    HANDLE hMyComm = NULL;

    //获取后台读命令的操作结果
    WaitForSingleObject(m_hRecvMsgEvent, INFINITE);   //使得线程自愿进入等待状态，直到变为已通知状态
    CloseHandle(m_hRecvMsgEvent);
    m_hRecvMsgEvent = NULL;

    while (m_bRunThread)   //接收任务运行标志为TRUE
    {
        if (m_hComm == INVALID_HANDLE_VALUE)
        {
            break;
        }

        ClearCommError(m_hComm, &dwErrorFlags, &ComStat);  //清除错误标志

        if (ComStat.cbInQue)
        {
            char szRecvBuf[MAXRECVBUF + 1] = { 0 };

            int nLen = 0;
            nLen = RecvData((void*)szRecvBuf, MAXRECVBUF);   //串口接收数据，放在szRecvBuf中

            if (nLen > 0)
            {
                m_RecvMsgCallFunc(szRecvBuf, nLen, m_pRegisterPoint); //回调函数
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
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
int CUart::SendData(LPCVOID pData, DWORD dwLen)
{
    //判断入口参数
    if (pData == NULL)
    {
        return ERR_DK_FAILURE;
    }

    BOOL bSuccess;
    DWORD dwErrorFlags;
    COMSTAT Comstat;
    DWORD dwSendCount = 0;
    DWORD dwError;

    ClearCommError(m_hComm, &dwErrorFlags, &Comstat);

    //发送数据
    bSuccess = WriteFile(m_hComm, pData, dwLen, &dwSendCount, &m_olSend);
    /*WriteFile函数：
    m_hComm : 写入文件的句柄
    pData   : 写入文件的数据缓冲区指针
    dwLen   : 写入的字节数
    dwSendCount : 已发送的字节数
    m_olSend: 重叠方式使用的结构体
    */

    if (!bSuccess)
    {
        //获得IO错误码
        dwError = GetLastError();
        if (ERROR_IO_PENDING == dwError)  //表示数据尚未发送完毕
        {
            //一直等待发送成功
            bSuccess = GetOverlappedResult(m_hComm, &m_olSend, &dwSendCount, TRUE);
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
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
int CUart::RecvData(LPVOID  pData, DWORD dwLen)
{
    //判断入口参数
    if (pData == NULL)
    {
        return ERR_DK_FAILURE;
    }

    BOOL bSuccess = FALSE;
    DWORD dwErrorFlags;
    COMSTAT Comstat;
    DWORD dwRecvCount = 0;
    DWORD dwError;

    ClearCommError(m_hComm, &dwErrorFlags, &Comstat);
    dwRecvCount = min(dwLen, Comstat.cbInQue);  //取实际接收的数据长度与接收缓冲长度之间的最小值

    //读事件发生
    bSuccess = ReadFile(m_hComm, pData, dwLen, &dwRecvCount, &m_olRecv);

    if (!bSuccess)
    {
        //获得IO错误码
        dwError = GetLastError();
        if (ERROR_IO_PENDING == dwError)  //表示数据尚未发送完毕
        {
            //一直等待接收成功
            bSuccess = GetOverlappedResult(m_hComm, &m_olRecv, &dwRecvCount, TRUE);
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
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
int CUart::WaitComm()
{
    OVERLAPPED os;
    DWORD dwMark = 0;
    DWORD dwTrans = 0;

    os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (os.hEvent == NULL)
    {
        return ERR_DK_FAILURE;
    }

    if (!WaitCommEvent(m_hComm, &dwMark, &os))   //重叠操作
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            //无限等待重叠操作结果
            GetOverlappedResult(m_hComm, &os, &dwTrans, TRUE);
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
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
void CUart::ClosePort()
{
    BOOL bOpen = FALSE;

    try
    {
        bOpen = IsOpen();
        m_bRunThread = FALSE;
        if (TRUE == bOpen)
        {
            //关闭当前打开的端口句柄
            (void)CloseHandle(m_hComm);
            m_hComm = INVALID_HANDLE_VALUE;

            //关闭当前事件句柄
            if (m_olSend.hEvent != NULL)
            {
                CloseHandle(m_olSend.hEvent);
            }

            if (m_olRecv.hEvent != NULL)
            {
                CloseHandle(m_olRecv.hEvent);
            }

            ZeroMemory(&m_olSend, sizeof(OVERLAPPED));
            ZeroMemory(&m_olRecv, sizeof(OVERLAPPED));
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
* 创建日期: 2017年08月29日
* 注    意:
*********************************************************************************************************/
BOOL CUart::IsOpen() const
{
    return  (INVALID_HANDLE_VALUE != m_hComm);
}



