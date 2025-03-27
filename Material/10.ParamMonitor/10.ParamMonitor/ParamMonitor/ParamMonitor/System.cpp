/*********************************************************************************************************
* ģ������: System.cpp
* ʹ��˵��: ϵͳ�󲿷�ȫ�ֱ������壻��ʼ���������壻�Լ�����ʾ���ܵ�ʵ�֣����ٵ��ڹ��ܵ�ʵ�֣�
* ժ    Ҫ: ϵͳ��ʼ���� �Լ�����ʾ���ܣ����ٵ��ڹ��ܣ�
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

/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "pch.h"
#include "System.h"
#include "UART.h"
#include "Procmsg.h"
#include "DisplayString.h"
#include "PackUnpack.h"
#include "Send.h"

/*********************************************************************************************************
*                                                �궨��
*********************************************************************************************************/

/*********************************************************************************************************
*                                               ȫ�ֱ���
*********************************************************************************************************/
//��������
StructUARTConfig gUARTConfig;

//����ָ��
CWnd *gWndP = NULL;  

//ʱ��
SYSTEMTIME gDateTime;

//�������á�״̬�����²���
StructTempStatus gTempStatus;
StructTempParams gTempParams;

//Ѫѹ���á�״̬��Ѫѹ����
StructNIBPStatus   gNIBPStatus;
StructNIBPParam    gNIBPParams;
StructNIBPCtrlInfo gNIBPCtrlInfo;

//Ѫ�����á�״̬��Ѫ������
StructSPO2Params gSPO2Params;
StructSPO2State  gStructSPO2State;

//�������á�״̬�ͺ�������
StructRespParams gRespParams;

//�ĵ����á�״̬���ĵ����
StructECGConfig gECGConfig;
StructECGStatus gECGStatus;
StructECGParams gECGParams;

const COLORREF  gColorOnOff[] = {
  RGB(255,0,0),    // ENUM OFF��Ӧ��ɫ����ʾ�������䣻
  RGB(0,255,0)     // ENUM ON��Ӧ��ɫ����ʾ����������
};

const static BOOL sInMeasuring[ ] =
{
	FALSE,  // NIBP_STATE_WAITING_MANUAL
	
	TRUE,   // NIBP_STATE_MANUAL_MEASURING
};

/*********************************************************************************************************
*                                               �ڲ�����
*********************************************************************************************************/

/*********************************************************************************************************
*                                               �ڲ�����
*********************************************************************************************************/
static void InitParam(void);
static void InitStatus(void);

/*********************************************************************************************************
*                                             �ڲ�����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: InitParam
* ��������: ��ʼ�������������еĲ�������Ϊ��Чֵ
* �������:
* �������:
* �� �� ֵ:
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
static void InitParam(void)
{
	gTempParams.t1 = INVALID_PARAM;          //����ֵ����
	gTempParams.t2 = INVALID_PARAM;

	gNIBPParams.sys       = INVALID_PARAM;   //Ѫѹֵ����
	gNIBPParams.mean      = INVALID_PARAM;
	gNIBPParams.dia       = INVALID_PARAM;
	gNIBPParams.pulseRate = INVALID_PARAM;

	gSPO2Params.cSPO2     = INVALID_PARAM;   //Ѫ��ֵ����
	gSPO2Params.pulseRate = INVALID_PARAM;

	for (int i = 0; i < 5; i++)
	{
		gSPO2Params.nSPO2Wave[i] = INVALID_PARAM;
	}

	gRespParams.rr        = INVALID_PARAM;   //����������

	gECGParams.hr         = INVALID_PARAM;

	gECGParams.wave1      = 2048;
	gECGParams.wave2      = 2048;
}

/*********************************************************************************************************
* ��������: InitStatus
* ��������: ��ʼ��״̬����
* �������:
* �������:
* �� �� ֵ:
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
static void InitStatus(void)
{
	gTempStatus.bLeadDetachedChl1 = TEMP_SENSOR_OFF;   //����̽ͷ1����״̬
	gTempStatus.bLeadDetachedChl2 = TEMP_SENSOR_OFF;

	gNIBPStatus.lastTestTime.wYear  = 0;
	gNIBPStatus.lastTestTime.wMonth = 0;
	gNIBPStatus.bCuffTypeWrong      = FALSE;
	gNIBPStatus.cuffPressure        = INVALID_PARAM;

	gNIBPStatus.status              = INVALID_PARAM;

	gStructSPO2State.bFingerShedding    = true;   //SPO2��ָ����
	gStructSPO2State.bSensorError       = true;   //SPO2����������״̬
	gStructSPO2State.bOvertimeSearching = false;  //SPO2����������Ϣ
	gStructSPO2State.bSPO2Falling       = false;  //Ѫ�����Ͷ��½�
	gStructSPO2State.signalIntensity    = 15;     //SPO2�ź�ǿ��:��Ч

	gECGStatus.bLeadDetachedRA      = true;       //RA����
	gECGStatus.bLeadDetachedLA      = true;       //LA����
	gECGStatus.bLeadDetachedLL      = true;       //LL����
	gECGStatus.bLeadDetachedRL      = true;       //RL����
	gECGStatus.bLeadDetachedV       = true;       //V ����
	
	gECGStatus.bOverload1           = false;      //�źŹ��أ���
	gECGStatus.bOverload2           = false;      //�źŹ��أ���
	gECGStatus.bCalibrating         = false;      //�Ƿ�����У׼����
}

/*********************************************************************************************************
* ��������: InitNIBPCtrlInfo
* ��������: NIBP������Ϣ��ʼ��
* �������:
* �������:
* �� �� ֵ:
* ��������: 2021��01��22��
* ע    ��:
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
*                                               һ�㺯��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: InitConfig
* ��������:	��ʼ��ϵͳ����
* �������:	void
* �������:	void
* �� �� ֵ: void
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
void InitConfig(void)
{
	gUARTConfig.nParity = 0;   // Ĭ��ѡ���0���У��
	gUARTConfig.nBaudRate = 6; // Ĭ��ѡ���6�115200
	gUARTConfig.nPort = 0;     // Ĭ��ѡ���1��˿�
	gUARTConfig.nDataBit = 3;  // Ĭ��ѡ���3�8λ����λ
	gUARTConfig.nStopBit = 0;  // Ĭ��ѡ���0�1λֹͣλ

	//��ʼ������Ϣ
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

	//�ĵ�
	gECGConfig.leadSystem = ECG_LEAD_5;            //5��
}

/*********************************************************************************************************
* ��������: InitializeSystem
* ��������: Ӧ�ó���ʼ����ʱ��������ʼ����ָ�������ʼ��
* �������:
* �������:
* �� �� ֵ:
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
void InitializeSystem(void) 
{
	InitMsgHandle();      //��Procmsg.cpp�ж���
	InitConfig();

	InitParam();
	InitStatus(); 

	InitNIBPCtrlInfo();
}

/*********************************************************************************************************
* ��������: IsShowCuffPressure
* ��������: ��ѯ��ʱ�Ƿ���Ҫ��ʾ���ѹ��
* �������:
* �������:
* �� �� ֵ: TRUE:��Ҫ;FALSE:����Ҫ
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
BOOL IsShowCuffPressure()
{
	return sInMeasuring[gNIBPCtrlInfo.iCurrentState];
}

/*********************************************************************************************************
* ��������: GetNIBPState
* ��������: ��ȡѪѹ״̬
* �������:
* �������:
* �� �� ֵ:
* ��������: 2021��01��22��
* ע    ��:
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
* ��������: SplitString
* ��������: ���ո����ķ��Ž����ַ����ķָ�
* �������: str-Ҫ��ֵ��ַ�����split-�����ķ��ţ�strArray-��ֺ���ַ�������
* �������:
* �� �� ֵ: �����ַ����ָ��ĸ���
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
int SplitString(const CString str, char split, CStringArray& strArray)
{
	strArray.RemoveAll();   //����
	CString strTemp = str;
	int nIndex = 0;
	while (1)
	{
		nIndex = strTemp.Find(split);  //�ҵ�ָ���ַ���λ��
		if (nIndex >= 0)
		{
			strArray.Add(strTemp.Left(nIndex));  //CString.left����߿�ʼ��ȡǰiIndex���ַ�
			strTemp = strTemp.Right(strTemp.GetLength() - nIndex - 1);
		}
		else
		{
			break;
		}
	}
	strArray.Add(strTemp);   //�������һ���ַ���

	return strArray.GetSize();
}  
