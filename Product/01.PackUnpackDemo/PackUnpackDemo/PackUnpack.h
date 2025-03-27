/*********************************************************************************************************
* ģ������: PackUnpack.h
* ʹ��˵��: ������ģ��
* ժ    Ҫ: ��ͷ�ļ�
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
*                                                ����ͷ�ļ�
*********************************************************************************************************/
#include "String.h"

/*********************************************************************************************************
*                                              ö�ٽṹ�嶨��
*********************************************************************************************************/
//���������鳤�ȣ�һ�������������ID������ʵ�ʳ��ȣ��������ݣ�����byte1����У���
enum{MAX_PACKET_LEN = 10};

//��֡���壺һ������һ���ֽڣ���һ���ֽڣ���8�ֽ�������ɣ�������ݰ�δ�������ֻ��7���ֽڵ�����
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
//ö�ٶ��壬����ID
typedef enum
{
	DAT_SYS     = 0x01,          //ϵͳ��Ϣ
	DAT_ECG     = 0x10,          //�ĵ���Ϣ
	DAT_RESP    = 0x11,          //������Ϣ
	DAT_TEMP    = 0x12,          //������Ϣ
	DAT_SPO2    = 0x13,          //Ѫ����Ϣ
	DAT_NBP     = 0x14,          //�޴�Ѫѹ��Ϣ
	DAT_IBP     = 0x15,          //�д�Ѫѹ��Ϣ
		
	MAX_PACK_ID = 0x80
}EnumPackID;

typedef enum 
{
	DAT_RST         = 0x01,
	DAT_SYS_STS     = 0x02,
	DAT_SELF_CHECK  = 0x03,
	DAT_CMD_ACK     = 0x04,
		
	DAT_ECG_WAVE    = 0x02,
	DAT_ECG_LEAD    = 0x03,
	DAT_ECG_HR      = 0x04,
	DAT_ST          = 0x05,
	DAT_ST_PAT      = 0x06,
		
	DAT_RESP_WAVE   = 0x02,
	DAT_RESP_RR     = 0x03,
	DAT_RESP_APNEA  = 0x04,
	DAT_RESP_CVA    = 0x05,
		
	DAT_TEMP_DATA   = 0x02,
		
	DAT_SPO2_WAVE   = 0x02,
	DAT_SPO2_DATA   = 0x03,
	
	DAT_NBP_CUFPRE  = 0x02,
	DAT_NBP_END     = 0x03,
	DAT_NBP_RSLT1   = 0x04,
	DAT_NBP_RSLT2   = 0x05,
	DAT_NBP_STS     = 0x06,  
}EnumSecondPackID;

typedef enum DataID
{
	DATA_ECG1_WAVE,
	DATA_ECG2_WAVE,
	DATA_ECG_HEART,
	DATA_ECG_LEAD_V,
	DATA_ECG_LEAD_RA,
	DATA_ECG_LEAD_LA,
	DATA_ECG_LEAD_LL,
		
	DATA_RESP_WAVE,
	DATA_RESP_RR,
		
	DATA_TEMP_VAL1,
	DATA_TEMP_VAL2,
	DATA_TEMP_STS1,
	DATA_TEMP_STS2,
	
	DATA_SPO2_WAVE,     //DATA_SPO2ֻ�������б��������ʵ��Ѫ��5�������Ǵ洢��s_arrUnpackData[DATA_SOP2_WAVE_0-5]����
	DATA_SPO2_WAVE_0,
	DATA_SPO2_WAVE_1,
	DATA_SPO2_WAVE_2,
	DATA_SPO2_WAVE_3,
	DATA_SPO2_WAVE_4,
		
	DATA_SPO2,
	DATA_PULSE,
	DATA_SPO2_LEAD,
		
	DATA_NBP_CUFPRE,
	DATA_NBP_END,
	DATA_NBP_SYS,		//��ѹ������ѹ
	DATA_NBP_DIA,		//��ѹ������ѹ
	DATA_NBP_MAP,		//ƽ��ѹ
	DATA_NBP_PR,		//����
		
	MAX_DATA_ID
}EnumDataID;

/*********************************************************************************************************
*                                              ��Ķ���
*********************************************************************************************************/
class CPackUnpack
{
public:
	//��Ա����
	CPackUnpack();
	virtual ~CPackUnpack();
	void InitPackUnpack(void);
	void PackData(PacketFrame* pack);
	BOOL UnpackData(UCHAR* data);
	void GetUnpackRslt(PacketFrame* pack);

private:
	PacketFrame mPack;
	UCHAR mPackLen;
	UCHAR mGotPackId;
	UCHAR mRestByteNum;
	void PackWithCheckSum(UCHAR* pack);
	BOOL UnpackWithCheckSum(UCHAR* pack);
};