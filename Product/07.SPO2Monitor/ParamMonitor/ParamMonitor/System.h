/*********************************************************************************************************
* ģ������: System.h
* ժ    Ҫ: ������ʹ�õ�ȫ�ֱ�����ȫ�ֺ�������
* ��ǰ�汾: 1.0.0
* ��    ��: Leyutek(COPYRIGHT 2018 - 2021 Leyutek. All rights reserved.)
* �������: 2021��01��22��
* ��    ��:
* ע    ��: none
**********************************************************************************************************
* ȡ���汾:
* ��    ��:
* �������:
* �޸�����:
* �޸��ļ�:
*********************************************************************************************************/
#pragma once

/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "DisplayString.h"

/*********************************************************************************************************
*                                                �궨��
*********************************************************************************************************/
// ����
#define  INVALID_PARAM_INT       (-100)
#define  INVALID_PARAM_FLOAT     (-100.0)
#define  INVALID_PARAM           (-100)
#define  INVALID_ST_PARAM        (-10000)

/*********************************************************************************************************
*                                             ö�ٽṹ�嶨��
*********************************************************************************************************/
//ϵͳ���е�������Ϣ���ݽṹ����
typedef struct
{  
	int   nPort;
	int   nBaudRate;
	int   nParity;   
	int   nDataBit;
	int   nStopBit;
}StructUARTConfig;

/*********************************************************************************************************
*                                                ȫ�ֱ���
*********************************************************************************************************/
extern StructUARTConfig gUARTConfig;

extern CWnd* gWndP;  //����ָ��

extern SYSTEMTIME gDateTime;

/*********************************************************************************************************
*                                                ������
*********************************************************************************************************/
//���ݰ����͵Ľṹ�嶨��
typedef enum
{
	PC_UNDEF = 0,     //δ���������
	PC_SEND,          //���Ͱ�
	PC_RECEIVE,       //���հ�
	PC_REPERR         //���Ͱ������󱨸����
}EnumPackClass;

//���������鳤�ȣ�һ�������������ID������ʵ�ʳ��ȣ��������ݣ�����byte1����У���
enum {MAX_PACKET_LEN = 10};

//��֡���壺һ������һ���ֽ�ID��һ���ֽڶ���ID,6���ֽ�����
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

//����Ϣ�����ȣ�����ID�����ǲ�����byte1������ݰ���<code>Pack</code>�����������
typedef struct
{  
	CHAR len;
	EnumPackClass  type;
	int        ackTime;  // <=0: ����Ҫȷ��
}StructFlowInfo;

//���ݰ�ģ��ID�ṹ�嶨��
enum  ModulePackID
{
	MODULEID_BASE  = 0,
	//MCU���͸�HOSTģ��ID
	DAT_SYS        = MODULEID_BASE + 0x01,         //ϵͳ��Ϣ
	DAT_ECG        = MODULEID_BASE + 0x10,         //�ĵ���Ϣ
	DAT_RESP       = MODULEID_BASE + 0x11,         //������Ϣ
	DAT_TEMP       = MODULEID_BASE + 0x12,         //������Ϣ
	DAT_SPO2       = MODULEID_BASE + 0x13,         //Ѫ����Ϣ
	DAT_NIBP       = MODULEID_BASE + 0x14,         //Ѫѹ��Ϣ
												   
	//HOST��MCU���������ģ��ID					   
	CMD_SYS        = MODULEID_BASE + 0x01,         //ϵͳ����
	CMD_ECG        = MODULEID_BASE + 0x10,         //�ĵ�����
	CMD_RESP       = MODULEID_BASE + 0x11,         //��������
	CMD_TEMP_M     = MODULEID_BASE + 0x12,         //��������
	CMD_SPO2_M     = MODULEID_BASE + 0x13,         //Ѫ������
	CMD_NIBP        = MODULEID_BASE + 0x14,         //Ѫѹ����
												   
	MODULEID_END,     							   
	MODULE_PACKETID_MAX    = 0x80,                 //���ID
};												   
												   
//���ݰ�����IDö�ٶ���						       
enum  SecondPackID								   
{												   
	//MCU���͸�HOST����ID					       							   
	DAT_ECG_BASE       = 0,						   
	DAT_ECG_WAVE       = DAT_ECG_BASE + 0x02,      //�ĵ粨������
	DAT_ECG_LEAD       = DAT_ECG_BASE + 0x03,      //�ĵ絼����Ϣ
	DAT_ECG_HR         = DAT_ECG_BASE + 0x04,      //����
	DAT_ECG_END,								   
												   
	DAT_RESP_BASE      = 0,						   
	DAT_RESP_WAVE      = DAT_RESP_BASE + 0x02,     //������������
	DAT_RESP_RR        = DAT_RESP_BASE + 0x03,     //������
	DAT_RESP_END,
	
	DAT_TEMP_BASE      = 0,
	DAT_TEMP_DATA      = DAT_TEMP_BASE + 0x02,     //��������
	DAT_TEMP_END,
	
	DAT_SPO2_BASE      = 0,
	DAT_SPO2_WAVE      = DAT_SPO2_BASE + 0x02,     //Ѫ������
	DAT_SPO2_DATA      = DAT_SPO2_BASE + 0x03,     //Ѫ������
	DAT_SPO2_END,
	
	DAT_NIBP_CUFPRE    = 0x02,           //�޴�Ѫѹʵʱ����
	DAT_NIBP_RSLT1     = 0x04,           //�޴�Ѫѹ�������1
	DAT_NIBP_RSLT2     = 0x05,           //�޴�Ѫѹ�������2
	
	//HOST���͸�MCU����ID
	CMD_NIBP_START      = 0x80,          //NIBP��������
	CMD_NIBP_END        = 0x81,          //NIBP��ֹ����
	CMD_NIBP_RST        = 0x84,          //NIBPģ�鸴λ

	CMD_NIBP_QUARY_STS  = 0x86,          //NIBP��ѯ״̬

	CMD_NIBP_RELT       = 0x89,          //NIBP��ѯ�ϴβ������
	
	SECONDID_END,
	PACKET_SECID_MAX    = 0xff           //����ID���
};

/*********************************************************************************************************
*                                                ���²���
*********************************************************************************************************/
typedef struct
{
	BOOL  bLeadDetachedChl1;      //����ͨ��1�����Ƿ����䣨TRUE�����䣩
	BOOL  bLeadDetachedChl2;      //����ͨ��2�����Ƿ����䣨TRUE�����䣩
}StructTempStatus;

typedef struct
{
	short  t1;     //ͨ��1����ֵ(����)
	short  t2;     //ͨ��2����ֵ(����)
}StructTempParams;

extern StructTempStatus gTempStatus;
extern StructTempParams gTempParams;

/*********************************************************************************************************
*                                                Ѫѹ����
*********************************************************************************************************/
typedef enum
{  
	NIBP_STATE_WAITING_MANUAL	= 0,
	NIBP_STATE_MANUAL_MEASURING = 1,
	NIBP_STATE_MAX
}EnumNIBPState;

//NIBP IDC_COMBO_MODE ����
typedef enum
{  
	NIBP_MANUAL   ,
	NIBP_CONTINUAL,
	NIBP_AUTO_MAX
}NIBPAUTOMEASUREPERIOD;

// NIBP������Ϣ
typedef enum
{  
	NIBP_CMD_START_STOP,
	NIBP_MSG_MEASURE_FINISHED,
	NIBP_CMD_MSG_MAX
}NIBPCMDMSG;

typedef struct
{
	short cuffPressure;        //�����ѹ��
	short status;              //NIBP״̬

	BOOL bCuffTypeWrong;       //������ʹ�TRUE���ǣ�
	SYSTEMTIME  lastTestTime;  //���һ�β���������ʱ��
}StructNIBPStatus;

typedef struct  
{
	short sys;				   //����ѹ
	short mean;                //ƽ��ѹ
	short dia;                 //����ѹ
	short pulseRate;           //����
	SYSTEMTIME testTime;       //����������ʱ��
}StructNIBPParam;

typedef struct  
{
	int     iPreviousState;
	int     iCurrentState;
	int     iLatestCmdMsg;

	BOOL    bShowCuffPressure;
	BOOL    bStart;            //TRUE����ʼ
}StructNIBPCtrlInfo;

extern StructNIBPStatus   gNIBPStatus;
extern StructNIBPParam    gNIBPParams;
extern StructNIBPCtrlInfo gNIBPCtrlInfo;

/*********************************************************************************************************
*                                                Ѫ������
*********************************************************************************************************/
//Ѫ�����Ͷ�״̬��Ϣ���½���־������ʱ���־���ź�ǿ�� 
typedef struct
{
	bool bFingerShedding;         //��ָ����״̬��־��1-����
	bool bSensorError;            //SPO2̽ͷ(������)����״̬��־��1-����
	bool bSPO2Falling;            //Ѫ�����Ͷ��½���־��1-�½�
	bool bOvertimeSearching;      //����ʱ��̫����־��1-����ʱ��>15s
	char signalIntensity;         //���������ź�ǿ�ȣ�0-8�� 15-��Чֵ
}StructSPO2State;

//Ѫ�������ṹ��
typedef struct
{
	char   cSPO2;            //Ѫ�����Ͷ� 0-100(%)  -100����Чֵ
	short  pulseRate;        //����  0-255 -100:��Чֵ
	short  nSPO2Wave[5];     //Ѫ���������ݣ�һ��������5������      
}StructSPO2Params;

extern StructSPO2Params gSPO2Params;
extern StructSPO2State  gStructSPO2State;

/*********************************************************************************************************
*                                                ��������
*********************************************************************************************************/

/*********************************************************************************************************
*                                                �ĵ����
*********************************************************************************************************/

/*********************************************************************************************************
*                                                ��������
*********************************************************************************************************/
void InitConfig(void);       //ϵͳ��ʼ������
void InitializeSystem(void); //ϵͳ��ʼ������
//���ո����ķ��Ž����ַ����ķָ�
int  SplitString(const CString str, char split, CStringArray &strArray);  

void InitNIBPCtrlInfo();     //NIBP������Ϣ��ʼ��
BOOL GetNIBPState(INT cmd, INT& state);  //��ȡNIBP״̬
BOOL IsShowCuffPressure();
