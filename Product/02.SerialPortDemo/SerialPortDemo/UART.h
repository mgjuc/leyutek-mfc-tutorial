/*********************************************************************************************************
* ģ������: UART.h
* ʹ��˵��:
* ժ    Ҫ: �궨��
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
#include "Global.h"

/*********************************************************************************************************
*                                               �궨��
*********************************************************************************************************/
//һ�㴮�ڵı�����"COM1","COM2",����openport�ӿ�����Ķ˿ڶ�����0��ʾCOM1��1��ʾCOM2
#define  MAP2PORT(port)   (port + 1)

/*********************************************************************************************************
*                                              ��Ķ���
*********************************************************************************************************/
class CUART
{
public:
	typedef int (*CALLFUNC)(void* pMsg, DWORD dwLength, void* pThis);  //CALLFUNC-�ص�����  

	enum EnumPortNum
	{
		COM1,
		COM2,
		COM3,
		COM4,
		COM5
	};

	enum EnumFlowControl     // ����������
	{
		No_FlowControl,      // ��������
		Cts_Rts_FlowControl, // ������/�������������
		Cts_Dtr_FlowControl,
		Dsr_Rts_FlowControl,
		Dsr_Dtr_FlowControl, // �����ն˾���/�������þ���������
		Xon_Xoff_FlowControl // ���������
	};

	enum EnumRecvMode
	{
		Call_Func_Mode, // �ص�����ģʽ
		Event_Mode      // �¼�����ģʽ
	};

public:
	CUART(); // ���캯��
	virtual ~CUART(); // ��������
	static unsigned _stdcall RecvThread(void* param); // �����������߳�

public:
	int OpenPort(int iPort, int iRecvMode = Call_Func_Mode); // ��һ���˿ڣ�����ʼ����Ĭ��CallFuncMode,����ʱ����ʡ��
	int ConfigPort(int baudRate = 0,
		int parity = 0,
		int dataBits = 3,
		int stopBits = 0,
		EnumFlowControl fc = No_FlowControl
	); // ���ö˿�
	void RegisterCallFunc(CALLFUNC callFunc, void* pThis); // ע�ắ��(�ص���������Ϊstatic) 
	int  RecvTaskProc(); // �����¼���Ϣ����
	int  SendData(LPCVOID pData, DWORD dwLen); // ��������
	int  RecvData(LPVOID  pData, DWORD dwLen); // ��������
	int  WaitComm(); // �ȴ������¼�
	void ClosePort(); // �رն˿�
	BOOL IsOpen() const; // �ж϶˿��Ƿ��
	HANDLE mCommHandle; // �˿ھ��

private:
	OVERLAPPED mLSend;           // �˿ڷ���OVERLAPPED�ṹ
	OVERLAPPED mLRecv;           // �˿ڽ���OVERLAPPED�ṹ
	CALLFUNC   mRecvMsgCallFunc; // ���ջص�����ָ��
	void*      mRegisterPoint;   // ע��ģ��ָ��
	int        mRecvMode;       
	HANDLE     mRecvMsgEvent;    // ������Ϣ֪ͨ�¼�������ص�ģʽ�ã�
	HANDLE     mRecvThread;      // �����߳̾�����ص�ģʽ�ã�
	BOOL       mRunThread;       // �����������б�־
};
