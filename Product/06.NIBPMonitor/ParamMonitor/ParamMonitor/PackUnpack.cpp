/*********************************************************************************************************
* 模块名称: PackUnpack.cpp
* 摘    要: 串口接收，发送，对数据的打包，解包；响应数据包，定时检查下位机的回答
* 当前版本: 1.0.0
* 作    者: Leyutek(COPYRIGHT 2018 - 2021 Leyutek. All rights reserved.)
* 完成日期: 2021年01月22日
* 内    容:
* 注    意:                                                                 
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
#include "PackUnpack.h"
#include "Procmsg.h"
#include "Global.h"
#include <shlwapi.h>    //用到PathIsDirectory函数
#pragma comment(lib, "shlwapi.lib")

/*********************************************************************************************************
*                                               内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                             成员函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: CPackUnpack
* 函数功能: 构造函数
* 输入参数: packInfo-包数组结构体{len, packType, ackTimes}， maxPackID-PackID的最大项
* 输出参数: 
* 返 回 值:  
* 创建日期: 2021年01月22日
* 注    意: regBaseAddr-mUART寄存器的基地址，initData-寄存器初始化序列为mUART
*********************************************************************************************************/
CPackUnpack::CPackUnpack(int maxPackID): mSynSection()
{
    mMaxPackID = maxPackID;
    
    mPacksReceived.Initialize( );   //接收PacketBuf初始化
    mPackIdGot     = 0;             //接收到包ID的标志位
    mCurrPackLen   = 0;
    mRestByte      = 0;             //剩余字节数
    mErrorPack     = 0;
}

/*********************************************************************************************************
* 函数名称: ~CPackUnpack
* 函数功能: 析构函数
* 输入参数: 
* 输出参数: 
* 返 回 值:  
* 创建日期: 2021年01月22日
* 注    意: 
*********************************************************************************************************/
CPackUnpack::~CPackUnpack( )
{

}

/*********************************************************************************************************
* 函数名称: Initialize
* 函数功能: packMan、PacketQueue初始化
* 输入参数: 
* 输出参数: 
* 返 回 值:  
* 创建日期: 2021年01月22日
* 注    意: 
*********************************************************************************************************/
void  CPackUnpack::Initialize( void )
{
    mPacksReceived.Initialize( );   //接收PacketBuf初始化
    mPackIdGot     = 0;             //接收到包ID的标志位
    mCurrPackLen   = 0;
    mRestByte      = 0;             //剩余字节数
    mErrorPack     = 0;
}

/*********************************************************************************************************
* 函数名称: UnpackWithCheckSum
* 函数功能: 带校验和的数据解包
* 输入参数: 待解包的数据pPack的首地址，打包后的数据长度
* 输出参数: 解包好的数据pPack的首地址
* 返 回 值: 0-解包不成功，1-解包成功 
* 创建日期: 2021年01月22日
* 注    意: 
*********************************************************************************************************/
bool CPackUnpack::UnpackWithCheckSum(UCHAR* pPack)
{
	UCHAR dataHead; //数据头
	int i;
	UCHAR checkSum;
	checkSum  = *(pPack);           //取出ID，加到校验和	
	dataHead = *(pPack + 1);        //取出数据包的数据头，赋给dataHead
	checkSum += dataHead;           //将数据头加到校验和
	
	for(i = 1; i < 8; i++)
	{
		checkSum += *(pPack + i + 1); //将数据依次加到校验和
		
		//还原有效的8位数据
		*(pPack+i) = (*(pPack + i + 1) & 0x7f) | ((dataHead & 0x1) << 7);
		
		dataHead >>= 1;               //数据头右移一位
	}
	
	//判断ID、数据头和数据求和的结果（低七位）是否与校验和的低七位相等，如果不等返回0
	if((checkSum & 0x7f ) != ((*(pPack + 9)) & 0x7f))
    {
        return false;
    }
	
    return true;
}

/*********************************************************************************************************
* 函数名称: PackWithCheckSum
* 函数功能: 带校验和的数据打包
* 输入参数: 待打包的数据pPack的首地址，打包后的数据长度（即未打包的数据长度+3） 
* 输出参数: 打包好的数据pPack的首地址	
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意: 数据头的BIT0为数据1的BIT7，数据头的BIT1为数据2的BIT7，数据头的BIT2为数据3的BIT7 
*********************************************************************************************************/
void CPackUnpack::PackWithCheckSum(UCHAR* pPack)
{
	int i;
	UCHAR dataHead; //数据头,在数据包的第二个位置，即ID之后
	UCHAR checkSum; //数据校验和，在数据包的最后一个位置
	checkSum = *(pPack);  //取出ID，加到校验和
    dataHead = 0;         //数据头清零
	for (i = 8; i > 1; i--) //数据包长度固定为10
	{
		//数据头左移,后面的数据的最高位位于dataHead的靠左
		dataHead <<= 1;
		//对数据进行最高位置1处理，并且将数据右移一位
		*(pPack + i) = *(pPack + i - 1) | 0x80;
		
		//数据加到校验和
		checkSum += *(pPack+i);
		
		//取出原始数据的最高位，与dataHead相或
		dataHead |= (*(pPack + i - 1) & 0x80) >> 7;
	}
	//数据头在数据包的第二个位置，仅次于包头，数据头的最高位也要置1
	*(pPack+1) = dataHead | 0x80;

	//将数据头加到校验和
	checkSum +=*(pPack+1);

	//校验和的最高位也要置为1
	*(pPack+9) = checkSum | 0x80;
}

/*********************************************************************************************************
* 函数名称: ReceivePack
* 函数功能: 从接收的数据流中分析解包
* 输入参数:  
* 输出参数: 
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意: 目前系统中接收数据和解包和数据包处理都是在一个线程中顺序处理，暂时不用考虑同步的问题
*********************************************************************************************************/
void  CPackUnpack::ReceivePack(UCHAR *pData, DWORD dwLen)
{
    int i;
    UCHAR currChar;
    bool result;
    
    //重复直到接收缓冲区中没有数据
    for(i = 0; i < dwLen; i ++)
    {
        currChar = *pData ++;
        
        //数据包ID已被接收
        if(mPackIdGot)
        {
            //当前字节是有效的分组数据
            if(0x80 <= currChar)
            {
                //注意：从第二个字节开始存储数据
                mCurrPack.buffer[mCurrPackLen] = currChar;
                mCurrPackLen ++;
                mRestByte --;
                
                //整个数据包被接收
                if(0 >= mRestByte)
                {
                    result = UnpackWithCheckSum(&mCurrPack.buffer[0]);
                    
                    if(result)  //成功解包
                    {
                        mPacksReceived.Put(mCurrPack);
                    }
                    else  //解包失败
                    {
                        mErrorPack ++;
                        ::PostMessage(gWndP->m_hWnd, WM_COMM_ERROR, mErrorPack, 0);
                    }
                    
                    mPackIdGot = false;
                }
              
            }
            else  //当前字节不是有效的数据包数据，也许是一个数据包ID
            {
                mErrorPack ++;
                //将一条消息放入到消息队列中。消息队列里的消息通过调用GetMessage和PeekMessage取得
                ::PostMessage(gWndP->m_hWnd, WM_COMM_ERROR, mErrorPack, 0);
                
                mPackIdGot = false;
            }
        }
        //整个数据包未被接收
        else
        {
            //检查currChar是否为有效的数据包ID
            if(mMaxPackID > currChar)
            {
                mRestByte     = 9;    //2个ID,6个数据
                mCurrPackLen  = 1;
                mCurrPack.id  = currChar;
                mPackIdGot  = true;
            }
            //currChar不是有效的数据包ID
            else
            {
                mErrorPack ++;     
                ::PostMessage(gWndP ->m_hWnd, WM_COMM_ERROR, mErrorPack, 0);
            }
        }
    }//循环结束
    
	return;
}

/*********************************************************************************************************
* 函数名称: SendPack
* 函数功能:	发送数据包
* 输入参数:	packSent-即将要发送的数据包
* 输出参数:	void	
* 返 回 值: 1-成功，0-失败
* 创建日期: 2021年01月22日
* 注    意: 
*********************************************************************************************************/ 
bool  CPackUnpack::SendPack(PacketFrame  packSent)
{
    int packLen;
    bool bFlag = false;
    int nRetVal = ERR_DK_FAILURE;  //发送串口数据返回值：成功返回发送字节数，失败返回ERR_DK_FAILURE
    
    mSynSection.Lock();
    
    packLen = gPacketFlowInfoPCT[packSent.id][packSent.id2].len;
    
    PackWithCheckSum(packSent.buffer);
    nRetVal = mUART.SendData(packSent.buffer, packLen);
    
    mSynSection.Unlock();
    
    if(ERR_DK_FAILURE != nRetVal)
    {
        bFlag = true;
    } 	
	return bFlag;
}

/*********************************************************************************************************
* 函数名称: IsPackData
* 函数功能:	检查给定的字符是否是合法的数据包数据
* 输入参数: data-数据要检查的字符
* 输出参数:	void	
* 返 回 值: true-合法，false-非法
* 创建日期: 2021年01月22日
* 注    意: 
*********************************************************************************************************/ 
bool CPackUnpack::IsPackData(UCHAR data)
{
    return (0x80 <= data);
}

/*********************************************************************************************************
* 函数名称: ReceiveProc
* 函数功能:	由于在关闭程序时，窗口（dialog和它的控件）先于CUART对象结构函数前关闭，所以此时
*           接收线程仍然在工作
* 输入参数: pData-接收的数据地址，dwLen-接收数据的长度，pThis-当前的指针
* 输出参数:	void	
* 返 回 值: true-合法，false-非法
* 创建日期: 2021年01月22日
* 注    意: 
*********************************************************************************************************/ 
int CPackUnpack::ReceiveProc(void *pData, DWORD dwLen, void *pThis)
{
    if(pThis)
    {
        ((CPackUnpack *)pThis)->ReceivePack((UCHAR *)pData, dwLen);   //解包
        
        ::ProcModuleMsg();   //使用主机命令的命令过程
        
        return DK_SUCCESS;
    }
    else
    {
        return ERR_DK_FAILURE;
    }
}

/*********************************************************************************************************
* 函数名称: OpenPort
* 函数功能:	打开串口，并启动接收线程
* 输入参数: port-串口号，nBaudRate-波特率，nParity-校验和
* 输出参数:	void	
* 返 回 值: 成功返回DK_SUCCESS
* 创建日期: 2021年01月22日
* 注    意: 
*********************************************************************************************************/ 
int CPackUnpack::OpenPort(int nPort, int nBaudRate, int nParity)
{
    if(!mUART.IsOpen())
    {
        mUART.OpenPort(nPort);
    }
    
    mUART.RegisterCallFunc(ReceiveProc, this);    //调用接收线程 
	return mUART.ConfigPort(nBaudRate, nParity, gUARTConfig.nDataBit, gUARTConfig.nStopBit);
}

