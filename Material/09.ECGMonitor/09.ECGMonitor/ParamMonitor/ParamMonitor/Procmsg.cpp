/*********************************************************************************************************
* 模块名称: procmsg.cpp
* 使用说明: 协议数据包处理函数定义；以及相关的数据定义；
* 摘    要: 对解析出来的数据包进行处理
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
#include "Procmsg.h"
#include "PackUnpack.h"
#include "ParamMonitorDlg.h"
#include "DisplayString.h"

/*********************************************************************************************************
*                                               全局变量
*********************************************************************************************************/
StructFlowInfo gPacketFlowInfoPCT[MODULE_PACKETID_MAX][PACKET_SECID_MAX];
CParamMonitorDlg* gPtrView = NULL;   // 全局的主view类指针；有关显示的所有操作都在其中；

/*********************************************************************************************************
*                                               内部变量
*********************************************************************************************************/
//定义一组函数指针数组，调用某一个函数时，通过ID，执行响应的函数
static MsgHandle sMsgHandle[MODULEID_END - MODULEID_BASE];

/*********************************************************************************************************
*                                             内部函数声明
*********************************************************************************************************/
static void DoNothing(UCHAR* msg );
static bool ProcUndefPack(PacketFrame& pack);
static bool ProcSendPack(PacketFrame& pack);
static bool ProcReceivePack(PacketFrame& pack);
static bool ProcReperrPack(PacketFrame& pack);
static void RegisterMsgHandle(UCHAR msg, MsgHandle msgHandler);

//模块ID响应
static void OnTemp                 ( UCHAR* msg );
static void OnNIBP                 ( UCHAR* msg );
static void OnSPO2                 ( UCHAR* msg );
static void OnResp                 ( UCHAR* msg );

//二级ID响应
static void OnTempData             ( UCHAR* msg );

//NIBP
static void OnNIBPData             ( UCHAR* msg );
static void OnNIBPResult           ( UCHAR* msg );

//SPO2
static void OnSPO2Wave             ( UCHAR* msg );
static void OnSPO2Data             ( UCHAR* msg );

//RESP
static void OnRespWave			   ( UCHAR* msg );
static void OnRespRr               ( UCHAR* msg );

/*********************************************************************************************************
*                                               内部函数
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: DoNothing
* 函数功能: 不实现任何功能
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void DoNothing(UCHAR* msg)
{
	NULL;
}

/*********************************************************************************************************
* 函数名称: ProcUndefPack
* 函数功能: 未定义数据包的命令过程
* 输入参数: pack-要处理的数据包
* 输出参数:
* 返 回 值: true-数据包已处理，false-数据包没有处理
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static bool ProcUndefPack(PacketFrame& pack)
{
	// nothing to do
	return true;
}

/*********************************************************************************************************
* 函数名称: ProcSendPack
* 函数功能: 发送数据包的命令过程。
* 输入参数: pack-要处理的数据包
* 输出参数:
* 返 回 值: true-数据包已处理，false-数据包没有处理
* 创建日期: 2021年01月22日
* 注    意: 发送数据包是从模块接收回来的数据包，通常它们是已响应的数据包。这是主从模块收到的返回包
*********************************************************************************************************/
static bool ProcSendPack(PacketFrame& pack)
{
	return true;
}

/*********************************************************************************************************
* 函数名称: ProcReceivePack
* 函数功能: 接收数据包的命令过程
* 输入参数: pack-要处理的数据包
* 输出参数:
* 返 回 值: true-数据包已处理，false-数据包没有处理
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static bool ProcReceivePack(PacketFrame& pack)
{
	//检查这个消息是否有msg处理程序
	if (gPacketFlowInfoPCT[pack.id][pack.id2].type == PC_RECEIVE)
	{
		//处理此消息
		//pack.id - RECEIVE_BASE-消息类型的ID，buffer[1]-二级ID首地址
		sMsgHandle[pack.id - MODULEID_BASE](&pack.buffer[1]);

		return true;
	}
	else
	{
		//在此发现错误
		return false;
	}
}

/*********************************************************************************************************
* 函数名称: ProcReperrPack
* 函数功能: 错误报告数据包的命令过程
* 输入参数: pack-要处理的数据包
* 输出参数:
* 返 回 值: true-数据包已处理，false-数据包没有处理
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static bool ProcReperrPack(PacketFrame& pack)
{
	return true;
}

/*********************************************************************************************************
* 函数名称: RegisterMsgHandle
* 函数功能:	注册消息处理函数
* 输入参数:	msg-消息类型，msgHandle-消息处理程序
* 输出参数:	void
* 返 回 值: true-成功，false-失败
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static  void  RegisterMsgHandle(UCHAR msg, MsgHandle msgHandler)
{
	if ((MODULEID_END > msg)
		&& (MODULEID_BASE <= msg)
		&& (NULL != msgHandler))
	{
		*(sMsgHandle + msg - MODULEID_BASE) = msgHandler;
	}
}

/*********************************************************************************************************
*                                                模块ID
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: OnTemp
* 函数功能: 处理体温数据
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnTemp(UCHAR* msg)
{
	switch(*msg)
	{
	case DAT_TEMP_DATA:
		OnTempData(msg + 1);   //除去二级ID，处理剩下的数据
		break;
	default:
		break;
	}
}

/*********************************************************************************************************
* 函数名称: OnNIBP
* 函数功能: 处理血压数据
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnNIBP(UCHAR* msg)
{
	gPtrView = (CParamMonitorDlg *)gWndP;
	switch(*msg)
	{
	case DAT_NIBP_CUFPRE:
		OnNIBPData(msg + 1); //除去二级ID，处理剩下的数据
		break;
	case DAT_NIBP_RSLT1:
		OnNIBPResult(msg + 1); //除去二级ID,显示血压结果1
		break;
	case DAT_NIBP_RSLT2:
		gNIBPParams.pulseRate = MAKEWORD( *(msg+2), *(msg+1) );
		break;
	default:
		break;
	}
}

/*********************************************************************************************************
* 函数名称: OnSPO2
* 函数功能: 处理血氧数据
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnSPO2(UCHAR* msg)
{
	switch(*msg)
	{
	case DAT_SPO2_WAVE:
		OnSPO2Wave(msg + 1); //除去二级ID，处理剩下的血氧波形数据
		break;
	case DAT_SPO2_DATA:
		OnSPO2Data(msg + 1); //除去二级ID，处理剩下的数据
		break;
	default:
		break;
	}
}

/*********************************************************************************************************
* 函数名称: OnResp
* 函数功能: 处理呼吸数据
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnResp(UCHAR* msg)
{
	switch (*msg)
	{
	case DAT_RESP_WAVE:
		if (NULL == gWndP) return;
		OnRespWave(msg + 1); //除去二级ID，处理剩下的呼吸波形数据
		break;
	case DAT_RESP_RR:
		OnRespRr(msg + 1);
		break;
	default:
		break;
	}
}

/*********************************************************************************************************
*                                                二级ID
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: OnTempData
* 函数功能: 处理体温数据
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnTempData(UCHAR* msg)
{
	gTempParams.t1 = MAKEWORD(*(msg + 2), *(msg + 1));               //体温值显示
	gTempParams.t2 = MAKEWORD(*(msg + 4), *(msg + 3));

	gTempStatus.bLeadDetachedChl1 = (*msg) & 0x01 ? true : false;   //体温连接状态
	gTempStatus.bLeadDetachedChl2 = (((*msg) & 0x02) >> 1) ? true : false;
}

/*********************************************************************************************************
* 函数名称: OnNIBPData
* 函数功能: 处理血压实时数据
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnNIBPData(UCHAR* msg)
{
	short cuffPres;
	CHAR  cuffError;
	INT   iMeasureMode;
	//袖带压力
	cuffPres = MAKEWORD( *(msg+1), *(msg) );
	if (INVALID_PARAM == cuffPres)
	{
		gNIBPStatus.cuffPressure = INVALID_PARAM;
	}
	else
	{
		if (0 <= cuffPres && 300 >= cuffPres)
		{
			gNIBPStatus.cuffPressure = cuffPres;
		}
		else
		{
			gNIBPStatus.cuffPressure = INVALID_PARAM;
		}
	}

	//袖带错误
	cuffError = *( msg + 2 );
	if (0 == cuffError)
	{
		gNIBPStatus.bCuffTypeWrong = FALSE;
	}
	else
	{
		if (1 == cuffError)
		{
			gNIBPStatus.bCuffTypeWrong = TRUE;
		}
		else
		{
			NULL; //接收数据错误
		}
	}
}

/*********************************************************************************************************
* 函数名称: OnNIBPResult
* 函数功能: 血压测量结果
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnNIBPResult(UCHAR* msg)
{
	gNIBPParams.sys      = MAKEWORD( *(msg+1), *(msg));
	gNIBPParams.dia      = MAKEWORD( *(msg+3), *(msg+2));
	gNIBPParams.mean     = MAKEWORD( *(msg+5), *(msg+4));
	gNIBPParams.testTime = gDateTime;

	gNIBPStatus.lastTestTime = gDateTime;
}

/*********************************************************************************************************
* 函数名称: OnSPO2Wave
* 函数功能:	处理血氧波形数据
* 输入参数:
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnSPO2Wave(UCHAR* msg)
{
	UCHAR i;
	UCHAR tmp = *(msg + 5);
	
	//一个包5个数据
	for(i = 0; i < 5; i++)
	{
		gSPO2Params.nSPO2Wave[i] = *(msg + i);
		gWndP ->PostMessage(WM_DRAW_SPO2_WAVE,  gSPO2Params.nSPO2Wave[i]);  
	}
	
	gStructSPO2State.bFingerShedding = (tmp >> 7) & 0x01;  //第7位
	gStructSPO2State.bSensorError = (tmp >> 4) & 0x01;     //第4位;
}

/*********************************************************************************************************
* 函数名称: OnSPO2Data
* 函数功能:	处理血氧数据
* 输入参数:	msg-
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnSPO2Data(UCHAR* msg)
{
	UCHAR tmp = *msg;
	gSPO2Params.pulseRate = MAKEWORD(*(msg + 2), *(msg + 1));
	gSPO2Params.cSPO2 = *(msg + 3);
	
	gStructSPO2State.bSPO2Falling = (tmp >> 5) & 0x01;        //第5位
	gStructSPO2State.bOvertimeSearching = (tmp >> 4) & 0x01;  //第4位
	gStructSPO2State.signalIntensity = tmp & 0x0f;            //第0-3位
}

/*********************************************************************************************************
* 函数名称: OnRespWave
* 函数功能:	处理呼吸波形数据
* 输入参数:
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnRespWave(UCHAR* msg)
{
	UCHAR i;
	for (i = 0 ; i < 5; i++)
	{
		gWndP ->PostMessage( WM_DRAW_RESP_WAVE , *( msg + i ));
	}
}

/*********************************************************************************************************
* 函数名称: OnRespRr
* 函数功能:	呼吸率
* 输入参数:
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void OnRespRr(UCHAR* msg)
{
	gRespParams.rr = MAKEWORD( *( msg+1 ) , *( msg ));
}

/*********************************************************************************************************
*                                               一般函数
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitMsgHandle
* 函数功能:	初始化主机消息的消息处理程序
* 输入参数:	void
* 输出参数:	void
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void  InitMsgHandle(void)
{
	//处理模块ID消息
	for (int i = 0; i < MODULEID_END - MODULEID_BASE; i++)
	{
		*(sMsgHandle + i) = DoNothing;
	}

	RegisterMsgHandle(DAT_TEMP, OnTemp);
	RegisterMsgHandle(DAT_NIBP, OnNIBP);
	RegisterMsgHandle(DAT_SPO2, OnSPO2);
	RegisterMsgHandle(DAT_RESP, OnResp);
}

/*********************************************************************************************************
* 函数名称: GetActiveWndPointer
* 函数功能:	初始化全局变量gWndP以接收进程功能使用,获取对话框的窗口类指针
* 输入参数:	void
* 输出参数:	void
* 返 回 值: 窗口的指针或NULL
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void GetActiveWndPointer(void* p)
{
	gWndP = p ? ((CWnd*)p) : NULL;
}

/*********************************************************************************************************
* 函数名称: ProcModuleMsg
* 函数功能:	使用主机命令的命令过程,从缓冲区取出数据包，并对数据包进行处理
* 输入参数:	void
* 输出参数:	void
* 返 回 值: void
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void  ProcModuleMsg(void)
{
	bool result = false;
	PacketFrame currPack;

	while (((CParamMonitorDlg*)gWndP)->mPackUnpack.mPacksReceived.Get(&currPack))  //从缓冲区取出数据包
	{
		switch (gPacketFlowInfoPCT[currPack.id][currPack.id2].type)//进行数据包的函数处理响应
		{
		case PC_UNDEF:
			ProcUndefPack(currPack);
			break;
		case PC_SEND:      //发送
			result = ProcSendPack(currPack);
			break;
		case PC_RECEIVE:   //接收
			result = ProcReceivePack(currPack);
			break;
		case PC_REPERR:
			result = ProcReperrPack(currPack);
			break;
		}
		if (result == false)
		{
			NULL;    //该命令尚未被处理，在此添加代码进行处理
		}
	}
}

