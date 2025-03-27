/*********************************************************************************************************
* ģ������: Global.h
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
*                                                �궨��
*********************************************************************************************************/
#define DK_SUCCESS          (0)  // �ɹ�
#define ERR_DK_FAILURE      (-1) // ʧ��
#define ERR_COM_NO          (-2) // �˿ںŴ���

const int MAXRECVBUF = 4096; // ���ڽ��ջ�����
const int MAXSENDBUF = 4096; // ���ڷ��ͻ�����

typedef unsigned char  uchar;

#define TARGET_RESOLUTION 1

// �����Ϊ��ʹ���Լ����Լ��ĵ��Թ���, ����ʹ��2ms�ļ��;
#define TIME_TICK  (20) // ��λ: ms; �����ý�嶨ʱ����ʱ����;

// Acknowledge time 
#define ACK_TIME  (200) // ָ�����Ӧʱ�䣬��λ: ms;

#define CUFFPREMAXVALUE 200
#define PULSEMAXVALUE 4096

#define WM_SECOND_TIMER	      (WM_USER+101)
#define WM_DRAW_CUffPre       (WM_USER+102)
#define WM_DRAW_Pulse         (WM_USER+103)
#define WM_MODULE_RESET       (WM_USER+105)
#define WM_COMMAND_ACK        (WM_USER+106)
#define WM_NIBP_RESET	      (WM_USER+107)
#define WM_NIBP_FINISH 	      (WM_USER+108)
#define WM_COMM_ERROR 		  (WM_USER+109)
#define WM_COMMAND_NOACK      (WM_USER+110)
#define APPLY_NIBP_FACTORY_CAL // Ӧ��NIBP���Ҳ���;

