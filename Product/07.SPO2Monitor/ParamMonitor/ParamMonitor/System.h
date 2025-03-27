/*********************************************************************************************************
* 模块名称: System.h
* 摘    要: 程序中使用的全局变量和全局函数定义
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
#include "DisplayString.h"

/*********************************************************************************************************
*                                                宏定义
*********************************************************************************************************/
// 参数
#define  INVALID_PARAM_INT       (-100)
#define  INVALID_PARAM_FLOAT     (-100.0)
#define  INVALID_PARAM           (-100)
#define  INVALID_ST_PARAM        (-10000)

/*********************************************************************************************************
*                                             枚举结构体定义
*********************************************************************************************************/
//系统所有的设置信息数据结构定义
typedef struct
{  
	int   nPort;
	int   nBaudRate;
	int   nParity;   
	int   nDataBit;
	int   nStopBit;
}StructUARTConfig;

/*********************************************************************************************************
*                                                全局变量
*********************************************************************************************************/
extern StructUARTConfig gUARTConfig;

extern CWnd* gWndP;  //窗口指针

extern SYSTEMTIME gDateTime;

/*********************************************************************************************************
*                                                打包解包
*********************************************************************************************************/
//数据包类型的结构体定义
typedef enum
{
	PC_UNDEF = 0,     //未定义的类型
	PC_SEND,          //发送包
	PC_RECEIVE,       //接收包
	PC_REPERR         //发送包（错误报告包）
}EnumPackClass;

//定义最大分组长度，一个分组包括分组ID，分组实际长度，分组数据（包括byte1）和校验和
enum {MAX_PACKET_LEN = 10};

//包帧定义：一个包由一个字节ID，一个字节二级ID,6个字节数据
typedef union
{  
	struct
	{
		UCHAR id;
		UCHAR id2;
		UCHAR data[MAX_PACKET_LEN - 2];
	};
	UCHAR buffer[MAX_PACKET_LEN];
}PacketFrame;

//包信息：长度（包括ID，但是不包括byte1如果数据包用<code>Pack</code>打包）和类型
typedef struct
{  
	CHAR len;
	EnumPackClass  type;
	int        ackTime;  // <=0: 不需要确认
}StructFlowInfo;

//数据包模块ID结构体定义
enum  ModulePackID
{
	MODULEID_BASE  = 0,
	//MCU发送给HOST模块ID
	DAT_SYS        = MODULEID_BASE + 0x01,         //系统信息
	DAT_ECG        = MODULEID_BASE + 0x10,         //心电信息
	DAT_RESP       = MODULEID_BASE + 0x11,         //呼吸信息
	DAT_TEMP       = MODULEID_BASE + 0x12,         //体温信息
	DAT_SPO2       = MODULEID_BASE + 0x13,         //血氧信息
	DAT_NIBP       = MODULEID_BASE + 0x14,         //血压信息
												   
	//HOST给MCU发送命令包模块ID					   
	CMD_SYS        = MODULEID_BASE + 0x01,         //系统命令
	CMD_ECG        = MODULEID_BASE + 0x10,         //心电命令
	CMD_RESP       = MODULEID_BASE + 0x11,         //呼吸命令
	CMD_TEMP_M     = MODULEID_BASE + 0x12,         //体温命令
	CMD_SPO2_M     = MODULEID_BASE + 0x13,         //血氧命令
	CMD_NIBP        = MODULEID_BASE + 0x14,         //血压命令
												   
	MODULEID_END,     							   
	MODULE_PACKETID_MAX    = 0x80,                 //最大ID
};												   
												   
//数据包二级ID枚举定义						       
enum  SecondPackID								   
{												   
	//MCU发送给HOST二级ID					       							   
	DAT_ECG_BASE       = 0,						   
	DAT_ECG_WAVE       = DAT_ECG_BASE + 0x02,      //心电波形数据
	DAT_ECG_LEAD       = DAT_ECG_BASE + 0x03,      //心电导联信息
	DAT_ECG_HR         = DAT_ECG_BASE + 0x04,      //心率
	DAT_ECG_END,								   
												   
	DAT_RESP_BASE      = 0,						   
	DAT_RESP_WAVE      = DAT_RESP_BASE + 0x02,     //呼吸波形数据
	DAT_RESP_RR        = DAT_RESP_BASE + 0x03,     //呼吸率
	DAT_RESP_END,
	
	DAT_TEMP_BASE      = 0,
	DAT_TEMP_DATA      = DAT_TEMP_BASE + 0x02,     //体温数据
	DAT_TEMP_END,
	
	DAT_SPO2_BASE      = 0,
	DAT_SPO2_WAVE      = DAT_SPO2_BASE + 0x02,     //血氧波形
	DAT_SPO2_DATA      = DAT_SPO2_BASE + 0x03,     //血氧数据
	DAT_SPO2_END,
	
	DAT_NIBP_CUFPRE    = 0x02,           //无创血压实时数据
	DAT_NIBP_RSLT1     = 0x04,           //无创血压测量结果1
	DAT_NIBP_RSLT2     = 0x05,           //无创血压测量结果2
	
	//HOST发送给MCU二级ID
	CMD_NIBP_START      = 0x80,          //NIBP启动测量
	CMD_NIBP_END        = 0x81,          //NIBP终止测量
	CMD_NIBP_RST        = 0x84,          //NIBP模块复位

	CMD_NIBP_QUARY_STS  = 0x86,          //NIBP查询状态

	CMD_NIBP_RELT       = 0x89,          //NIBP查询上次测量结果
	
	SECONDID_END,
	PACKET_SECID_MAX    = 0xff           //二级ID最大
};

/*********************************************************************************************************
*                                                体温测量
*********************************************************************************************************/
typedef struct
{
	BOOL  bLeadDetachedChl1;      //体温通道1导联是否脱落（TRUE：脱落）
	BOOL  bLeadDetachedChl2;      //体温通道2导联是否脱落（TRUE：脱落）
}StructTempStatus;

typedef struct
{
	short  t1;     //通道1体温值(摄氏)
	short  t2;     //通道2体温值(摄氏)
}StructTempParams;

extern StructTempStatus gTempStatus;
extern StructTempParams gTempParams;

/*********************************************************************************************************
*                                                血压测量
*********************************************************************************************************/
typedef enum
{  
	NIBP_STATE_WAITING_MANUAL	= 0,
	NIBP_STATE_MANUAL_MEASURING = 1,
	NIBP_STATE_MAX
}EnumNIBPState;

//NIBP IDC_COMBO_MODE 设置
typedef enum
{  
	NIBP_MANUAL   ,
	NIBP_CONTINUAL,
	NIBP_AUTO_MAX
}NIBPAUTOMEASUREPERIOD;

// NIBP控制信息
typedef enum
{  
	NIBP_CMD_START_STOP,
	NIBP_MSG_MEASURE_FINISHED,
	NIBP_CMD_MSG_MAX
}NIBPCMDMSG;

typedef struct
{
	short cuffPressure;        //袖带内压力
	short status;              //NIBP状态

	BOOL bCuffTypeWrong;       //袖带类型错（TRUE：是）
	SYSTEMTIME  lastTestTime;  //最后一次测量的日期时间
}StructNIBPStatus;

typedef struct  
{
	short sys;				   //收缩压
	short mean;                //平均压
	short dia;                 //舒张压
	short pulseRate;           //脉率
	SYSTEMTIME testTime;       //测量的日期时间
}StructNIBPParam;

typedef struct  
{
	int     iPreviousState;
	int     iCurrentState;
	int     iLatestCmdMsg;

	BOOL    bShowCuffPressure;
	BOOL    bStart;            //TRUE：开始
}StructNIBPCtrlInfo;

extern StructNIBPStatus   gNIBPStatus;
extern StructNIBPParam    gNIBPParams;
extern StructNIBPCtrlInfo gNIBPCtrlInfo;

/*********************************************************************************************************
*                                                血氧测量
*********************************************************************************************************/
//血氧饱和度状态信息：下降标志、搜索时间标志、信号强度 
typedef struct
{
	bool bFingerShedding;         //手指脱落状态标志：1-脱落
	bool bSensorError;            //SPO2探头(传感器)脱落状态标志：1-脱落
	bool bSPO2Falling;            //血氧饱和度下降标志：1-下降
	bool bOvertimeSearching;      //搜索时间太长标志：1-搜索时间>15s
	char signalIntensity;         //脉搏搏动信号强度：0-8， 15-无效值
}StructSPO2State;

//血氧参数结构体
typedef struct
{
	char   cSPO2;            //血氧饱和度 0-100(%)  -100：无效值
	short  pulseRate;        //脉率  0-255 -100:无效值
	short  nSPO2Wave[5];     //血氧波形数据，一个包中有5个数据      
}StructSPO2Params;

extern StructSPO2Params gSPO2Params;
extern StructSPO2State  gStructSPO2State;

/*********************************************************************************************************
*                                                呼吸测量
*********************************************************************************************************/

/*********************************************************************************************************
*                                                心电测量
*********************************************************************************************************/

/*********************************************************************************************************
*                                                函数声明
*********************************************************************************************************/
void InitConfig(void);       //系统初始化函数
void InitializeSystem(void); //系统初始化函数
//按照给定的符号进行字符串的分割
int  SplitString(const CString str, char split, CStringArray &strArray);  

void InitNIBPCtrlInfo();     //NIBP控制信息初始化
BOOL GetNIBPState(INT cmd, INT& state);  //获取NIBP状态
BOOL IsShowCuffPressure();
