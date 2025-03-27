/*********************************************************************************************************
* 模块名称: System.cpp
* 使用说明: 系统大部分全局变量定义；初始化函数定义；自检结果显示功能的实现；波速调节功能的实现；
* 摘    要: 系统初始化； 自检结果显示功能；波速调节功能；
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
#include "Procmsg.h"
#include "DisplayString.h"
#include "PackUnpack.h"
#include "Send.h"

/*********************************************************************************************************
*                                                宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                               全局变量
*********************************************************************************************************/
//串口配置
StructUARTConfig gUARTConfig;

//窗口指针
CWnd *gWndP = NULL;  

//时间
SYSTEMTIME gDateTime;

//体温配置、状态和体温参数
StructTempStatus gTempStatus;
StructTempParams gTempParams;

//血压配置、状态和血压参数
StructNIBPStatus   gNIBPStatus;
StructNIBPParam    gNIBPParams;
StructNIBPCtrlInfo gNIBPCtrlInfo;

//血氧配置、状态和血氧参数
StructSPO2Params gSPO2Params;
StructSPO2State  gStructSPO2State;

//呼吸配置、状态和呼吸参数
StructRespParams gRespParams;

//心电配置、状态和心电参数
StructECGConfig gECGConfig;
StructECGStatus gECGStatus;
StructECGParams gECGParams;

const COLORREF  gColorOnOff[] = {
  RGB(255,0,0),    // ENUM OFF对应红色；表示导联脱落；
  RGB(0,255,0)     // ENUM ON对应绿色；表示导联正常；
};

const static BOOL sInMeasuring[ ] =
{
	FALSE,  // NIBP_STATE_WAITING_MANUAL
	
	TRUE,   // NIBP_STATE_MANUAL_MEASURING
};

/*********************************************************************************************************
*                                               内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                               内部函数
*********************************************************************************************************/
static void InitParam(void);
static void InitStatus(void);

/*********************************************************************************************************
*                                             内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitParam
* 函数功能: 初始化参数，把所有的参数设置为无效值
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void InitParam(void)
{
	gTempParams.t1 = INVALID_PARAM;          //体温值设置
	gTempParams.t2 = INVALID_PARAM;

	gNIBPParams.sys       = INVALID_PARAM;   //血压值设置
	gNIBPParams.mean      = INVALID_PARAM;
	gNIBPParams.dia       = INVALID_PARAM;
	gNIBPParams.pulseRate = INVALID_PARAM;

	gSPO2Params.cSPO2     = INVALID_PARAM;   //血氧值设置
	gSPO2Params.pulseRate = INVALID_PARAM;

	for (int i = 0; i < 5; i++)
	{
		gSPO2Params.nSPO2Wave[i] = INVALID_PARAM;
	}

	gRespParams.rr        = INVALID_PARAM;   //呼吸率设置

	gECGParams.hr         = INVALID_PARAM;

	gECGParams.wave1      = 2048;
	gECGParams.wave2      = 2048;
}

/*********************************************************************************************************
* 函数名称: InitStatus
* 函数功能: 初始化状态变量
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
static void InitStatus(void)
{
	gTempStatus.bLeadDetachedChl1 = TEMP_SENSOR_OFF;   //体温探头1连接状态
	gTempStatus.bLeadDetachedChl2 = TEMP_SENSOR_OFF;

	gNIBPStatus.lastTestTime.wYear  = 0;
	gNIBPStatus.lastTestTime.wMonth = 0;
	gNIBPStatus.bCuffTypeWrong      = FALSE;
	gNIBPStatus.cuffPressure        = INVALID_PARAM;

	gNIBPStatus.status              = INVALID_PARAM;

	gStructSPO2State.bFingerShedding    = true;   //SPO2手指脱落
	gStructSPO2State.bSensorError       = true;   //SPO2传感器连接状态
	gStructSPO2State.bOvertimeSearching = false;  //SPO2搜索脉搏信息
	gStructSPO2State.bSPO2Falling       = false;  //血氧饱和度下降
	gStructSPO2State.signalIntensity    = 15;     //SPO2信号强度:无效

	gECGStatus.bLeadDetachedRA      = true;       //RA脱落
	gECGStatus.bLeadDetachedLA      = true;       //LA脱落
	gECGStatus.bLeadDetachedLL      = true;       //LL脱落
	gECGStatus.bLeadDetachedRL      = true;       //RL脱落
	gECGStatus.bLeadDetachedV       = true;       //V 脱落
	
	gECGStatus.bOverload1           = false;      //信号过载：否
	gECGStatus.bOverload2           = false;      //信号过载：否
	gECGStatus.bCalibrating         = false;      //是否正在校准：否
}

/*********************************************************************************************************
* 函数名称: InitNIBPCtrlInfo
* 函数功能: NIBP控制信息初始化
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void InitNIBPCtrlInfo()
{
	gNIBPCtrlInfo.iPreviousState       = NIBP_STATE_WAITING_MANUAL;
	gNIBPCtrlInfo.iCurrentState        = NIBP_STATE_WAITING_MANUAL;
	gNIBPCtrlInfo.iLatestCmdMsg        = NIBP_CMD_START_STOP;

	gNIBPCtrlInfo.bShowCuffPressure    = FALSE;
	gNIBPCtrlInfo.bStart               = FALSE;
}


/*********************************************************************************************************
*                                               一般函数
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
	gUARTConfig.nParity = 0;   // 默认选择第0项：无校验
	gUARTConfig.nBaudRate = 6; // 默认选择第6项：115200
	gUARTConfig.nPort = 0;     // 默认选择第1项端口
	gUARTConfig.nDataBit = 3;  // 默认选择第3项：8位数据位
	gUARTConfig.nStopBit = 0;  // 默认选择第0项：1位停止位

	//初始化包信息
	//ECG
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_WAVE].len     = 10;
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_WAVE].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_WAVE].ackTime = 1;
	
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_LEAD].len     = 10;
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_LEAD].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_LEAD].ackTime = 0;
	
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_HR].len     = 10;
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_HR].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_ECG][DAT_ECG_HR].ackTime = 0;
	
	//RESP
	gPacketFlowInfoPCT[DAT_RESP][DAT_RESP_WAVE].len     = 10;
	gPacketFlowInfoPCT[DAT_RESP][DAT_RESP_WAVE].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_RESP][DAT_RESP_WAVE].ackTime = 0;
	
	gPacketFlowInfoPCT[DAT_RESP][DAT_RESP_RR].len     = 10;
	gPacketFlowInfoPCT[DAT_RESP][DAT_RESP_RR].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_RESP][DAT_RESP_RR].ackTime = 0;
	
	//Temp
	gPacketFlowInfoPCT[DAT_TEMP][DAT_TEMP_DATA].len     = 10;
	gPacketFlowInfoPCT[DAT_TEMP][DAT_TEMP_DATA].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_TEMP][DAT_TEMP_DATA].ackTime = 0;
	
	//SPO2
	gPacketFlowInfoPCT[DAT_SPO2][DAT_SPO2_WAVE].len     = 10;
	gPacketFlowInfoPCT[DAT_SPO2][DAT_SPO2_WAVE].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_SPO2][DAT_SPO2_WAVE].ackTime = 0;
	
	gPacketFlowInfoPCT[DAT_SPO2][DAT_SPO2_DATA].len     = 10;
	gPacketFlowInfoPCT[DAT_SPO2][DAT_SPO2_DATA].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_SPO2][DAT_SPO2_DATA].ackTime = 0;
	
	//NIBP
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_CUFPRE].len     = 10;
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_CUFPRE].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_CUFPRE].ackTime = 0;
	
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_RSLT1].len     = 10;
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_RSLT1].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_RSLT1].ackTime = 0;
	
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_RSLT2].len     = 10;
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_RSLT2].type    = PC_RECEIVE;
	gPacketFlowInfoPCT[DAT_NIBP][DAT_NIBP_RSLT2].ackTime = 0;
	
	//NIBP
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_START].len     = 10;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_START].type    = PC_SEND;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_START].ackTime = ACK_TIME/TIME_TICK;
	
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_END].len     = 10;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_END].type    = PC_SEND;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_END].ackTime = ACK_TIME/TIME_TICK;
	
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_RST].len     = 10;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_RST].type    = PC_SEND;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_RST].ackTime = ACK_TIME/TIME_TICK;
	
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_QUARY_STS].len     = 10;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_QUARY_STS].type    = PC_SEND;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_QUARY_STS].ackTime = ACK_TIME/TIME_TICK;
	
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_RELT].len     = 10;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_RELT].type    = PC_SEND;
	gPacketFlowInfoPCT[CMD_NIBP][CMD_NIBP_RELT].ackTime = ACK_TIME/TIME_TICK;

	//心电
	gECGConfig.leadSystem = ECG_LEAD_5;            //5导
}

/*********************************************************************************************************
* 函数名称: InitializeSystem
* 函数功能: 应用程序开始运行时，变量初始化，指针变量初始化
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
void InitializeSystem(void) 
{
	InitMsgHandle();      //在Procmsg.cpp中定义
	InitConfig();

	InitParam();
	InitStatus(); 

	InitNIBPCtrlInfo();
}

/*********************************************************************************************************
* 函数名称: IsShowCuffPressure
* 函数功能: 查询此时是否需要显示袖带压力
* 输入参数:
* 输出参数:
* 返 回 值: TRUE:需要;FALSE:不需要
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
BOOL IsShowCuffPressure()
{
	return sInMeasuring[gNIBPCtrlInfo.iCurrentState];
}

/*********************************************************************************************************
* 函数名称: GetNIBPState
* 函数功能: 获取血压状态
* 输入参数:
* 输出参数:
* 返 回 值:
* 创建日期: 2021年01月22日
* 注    意:
*********************************************************************************************************/
BOOL GetNIBPState(INT cmd, INT &state)
{
	INT status;

	if ( NIBP_MSG_MEASURE_FINISHED >= cmd &&
		0 <= cmd &&
		NIBP_STATE_MAX > state &&
		0 <= state)
	{
		status = NIBP_STATE_MANUAL_MEASURING;

		state = status;
		return TRUE;
	}
	else
	{
	  state = NIBP_STATE_MAX;
	  return FALSE;
	}
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
	strArray.RemoveAll();   //清零
	CString strTemp = str;
	int nIndex = 0;
	while (1)
	{
		nIndex = strTemp.Find(split);  //找到指定字符的位置
		if (nIndex >= 0)
		{
			strArray.Add(strTemp.Left(nIndex));  //CString.left从左边开始获取前iIndex个字符
			strTemp = strTemp.Right(strTemp.GetLength() - nIndex - 1);
		}
		else
		{
			break;
		}
	}
	strArray.Add(strTemp);   //增加最后一个字符串

	return strArray.GetSize();
}  
